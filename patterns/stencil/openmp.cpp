#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;

// Represents a pixel as a triple of intensities
struct pixel {
    double red;
    double green;
    double blue;

    pixel(double r, double g, double b) : red(r), green(g), blue(b) {};
};

/*
 * The Prewitt kernels can be applied after a blur to help highlight edges
 * The input image must be gray scale/intensities:
 *     double intensity = (in[in_offset].red + in[in_offset].green + in[in_offset].blue)/3.0;
 * Each kernel must be applied to the blured images separately and then composed:
 *     blurred[i] with prewittX -> Xedges[i]
 *     blurred[i] with prewittY -> Yedges[i]
 *     outIntensity[i] = sqrt(Xedges[i]*Xedges[i] + Yedges[i]*Yedges[i])
 * To turn the out intensity to an out color set each color to the intensity
 *     out[i].red = outIntensity[i]
 *     out[i].green = outIntensity[i]
 *     out[i].blue = outIntensity[i]
 *
 * For more on the Prewitt kernels and edge detection:
 *     http://en.wikipedia.org/wiki/Prewitt_operator
 */
void prewittX_kernel(const int rows, const int cols, double * const kernel) {
    if(rows != 3 || cols !=3) {
        std::cerr << "Bad Prewitt kernel matrix\n";
        return;
    }
    for(int i=0;i<3;i++) {
        kernel[0 + (i*rows)] = -1.0;
        kernel[1 + (i*rows)] = 0.0;
        kernel[2 + (i*rows)] = 1.0;
    }
}

void prewittY_kernel(const int rows, const int cols, double * const kernel) {
        if(rows != 3 || cols !=3) {
                std::cerr << "Bad Prewitt kernel matrix\n";
                return;
        }
        for(int i=0;i<3;i++) {
                kernel[i + (0*rows)] = 1.0;
                kernel[i + (1*rows)] = 0.0;
                kernel[i + (2*rows)] = -1.0;
        }
}

/*
 * The gaussian kernel provides a stencil for blurring images based on a
 * normal distribution
 * The kernel will provide a template for the contribution of near by
 * pixels to a pixel under consideration.
 */
void gaussian_kernel(const int rows, const int cols, const double stddev, double * const kernel) {
    // values needed for the curve
    const double denom = 2.0 * stddev * stddev;
    const double g_denom = M_PI * denom;
    const double g_denom_recip = (1.0/g_denom);
    // accumulator so that values can be normalized to 1
    double sum = 0.0;

    // Build the template
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            const double row_dist = i - (rows/2);
            const double col_dist = j - (cols/2);
            const double dist_sq = (row_dist * row_dist) + (col_dist * col_dist);
            const double value = g_denom_recip * exp((-dist_sq)/denom);
            kernel[i + (j*rows)] = value;
            sum += value;
        }
    }

    // Normalize
    const double recip_sum = 1.0 / sum;
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            kernel[i + (j*rows)] *= recip_sum;
        }
    }
}

/*
 * Applies a gaussian blur stencil to an image
 */
void apply_stencil(const int radius, const double stddev, const int rows, const int cols, pixel * const in, pixel * const out, int stencil) {
    const int dim = radius*2+1;
    double kernel[dim*dim];
    double kernel2[dim*dim];
    if stencil == 0{
      gaussian_kernel(dim, dim, stddev, kernel);
    }
    else{
      prewittX_kernel(dim, dim, kernel);
      prewittY_kernel(dim, dim, kernel2);
    }
    // For each pixel in the image...
    #pragma omp parallel for
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            const int out_offset = i + (j*rows);
            // ...apply the template centered on the pixel...
            for(int x = i - radius, kx = 0; x <= i + radius; ++x, ++kx) {
                for(int y = j - radius, ky = 0; y <= j + radius; ++y, ++ky) {
                    // ...and skip parts of the template outside of the image
                    if(x >= 0 && x < rows && y >= 0 && y < cols) {
                        // Acculate intensities in the output pixel
                        const int in_offset = x + (y*rows);
                        const int k_offset = kx + (ky*dim);
                        if(stencil == 0){
                          out[out_offset].red   += kernel[k_offset] * in[in_offset].red;
                          out[out_offset].green += kernel[k_offset] * in[in_offset].green;
                          out[out_offset].blue  += kernel[k_offset] * in[in_offset].blue;
                          // double intensity = (out[out_offset].red + out[out_offset].green + out[out_offset].blue)/3.0;
                          // out[out_offset].red = intensity;
                          // out[out_offset].green = intensity;
                          // out[out_offset].blue  = intensity;
                        }
                        else{
                          double intensity = (in[in_offset].red + in[in_offset].green + in[in_offset].blue)/3.0;
                          double xEdge = kernel[k_offset] * intensity;
                          double yEdge = kernel2[k_offset] * intensity;
                          double outIntensity = sqrt(xEdge*xEdge + yEdge*yEdge);
                          out[out_offset].red = outIntensity;
                          out[out_offset].green = outIntensity;
                          out[out_offset].blue = outIntensity;
                        }
                    }
                }
            }
        }
    }
}

int main( int argc, char* argv[] ) {

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " imageName\n";
        return 1;
    }

    // Read image
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if(!image.data ) {
        std::cout <<  "Error opening " << argv[1] << std::endl;
        return -1;
    }

    // Get image into C array of doubles for processing
    const int rows = image.rows;
    const int cols = image.cols;
    pixel * imagePixels = (pixel *) malloc(rows * cols * sizeof(pixel));
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            Vec3b p = image.at<Vec3b>(i, j);
            imagePixels[i + (j*rows)] = pixel(p[0]/255.0,p[1]/255.0,p[2]/255.0);
        }
    }

    // Create output array 1
    pixel * outPixels1 = (pixel *) malloc(rows * cols * sizeof(pixel));
    for(int i = 0; i < rows * cols; ++i) {
        outPixels1[i].red = 0.0;
        outPixels1[i].green = 0.0;
        outPixels1[i].blue = 0.0;
    }

    // Create output array 2
    pixel * outPixels2 = (pixel *) malloc(rows * cols * sizeof(pixel));
    for(int i = 0; i < rows * cols; ++i) {
        outPixels2[i].red = 0.0;
        outPixels2[i].green = 0.0;
        outPixels2[i].blue = 0.0;
    }

    // Do the stencil
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    apply_stencil(3, 32.0, rows, cols, imagePixels, outPixels1, 0);
    apply_stencil(3, 32.0, rows, cols, outPixels1, outPixels2, 1);
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    printf("Stencil application took %dms\n",msec);

    // Create an output image (same size as input)
    Mat dest(rows, cols, CV_8UC3);
    // Copy C array back into image for output
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            const size_t offset = i + (j*rows);
            dest.at<Vec3b>(i, j) = Vec3b(floor(outPixels2[offset].red * 255.0),
                                         floor(outPixels2[offset].green * 255.0),
                                         floor(outPixels2[offset].blue * 255.0));
        }
    }

    imwrite("out.jpg", dest);


    free(imagePixels);
    free(outPixels2);
    return 0;
}
