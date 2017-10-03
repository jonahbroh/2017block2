/**
 * Program to scale and project data into 2D and find a centroid
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* struct to hold objects attributes */
struct phaseballx{
    float x;
};

struct phasebally{
    float y;
};

struct phaseballz{
    float z;
};

struct phaseballmass{
    float mass;
};

struct phaseball {
    struct phaseballx** x;
    struct phasebally** y;
    struct phaseballz** z;
    struct phaseballmass** mass;
};

struct volume {
    size_t size;
    size_t last;
    struct phaseball* objects;
};

// Add phaseball to a volume
void volume_append(struct volume* v, struct phaseball* o, int total) {
    (v->objects) = o;
    (v->size) = total*100;
    (v->last) = total+1;
    // if( v->last == v->size ) {
    //     (v->size) += 100;
    //     v->objects = realloc(v->objects, sizeof(struct phaseball*)*(v->size)+100);
    // }
    // (v->objects)[(v->last)] = o;
    // (v->last) += 1;
    return;
}

// Place phaseballs uniformly in a box, with mass equal to the manhattan distance
void place_uniformly(int sx, int ex, int sy, int ey, int sz, int ez, struct volume* v) {
  int total = 0;
  struct phaseball* n = malloc(sizeof(struct phaseball));
  n->x = malloc(sizeof(struct phaseballx[(ex-sx)*(ey-sy)*(ez-sz)]));
  n->y = malloc(sizeof(struct phasebally[(ex-sx)*(ey-sy)*(ez-sz)]));
  n->z = malloc(sizeof(struct phaseballz[(ex-sx)*(ey-sy)*(ez-sz)]));
  n->mass = malloc(sizeof(struct phaseballmass[(ex-sx)*(ey-sy)*(ez-sz)]));
    for(int i=sx; i<=ex; i++) {
        for(int j=sy; j<=ey; j++) {
            for(int k=sz; k<=ez; k++) {
                n->x[total]->x = i;
                n->y[total]->y = j;
                n->z[total]->z = k;
                n->mass[total]->mass = fabs(n->x[total]->x)+fabs(n->y[total]->y)+fabs(n->z[total]->z);
                total += 1;
            }
        }
    }
    volume_append(v,n,total);
}

// Projects 3D volume to 11x11 2D map and report centroid
void post_process(struct volume* v, float* cx, float* cy) {
    double mass_sum=0.0;
    double wx=0.0;
    double wy=0.0;
    struct phaseball* o = v->objects;
    for(int i=0; i<v->last; i++) {
        mass_sum += o->mass[i]->mass;
        wx += o->x[i]->x * o->mass[i]->mass;
        wy += o->y[i]->y * o->mass[i]->mass;
    }
    *cx = wx/mass_sum;
    *cy = wy/mass_sum;

    return;
}

int main(int argc, char** argv) {
    // make a volume to store objects in
    struct volume v;
    v.size=100;
    v.last=0;
    v.objects = malloc(sizeof(struct phaseball*)*100);

    // Set the initial configuration
    place_uniformly(-1000,1000,-100,100,-100,100,&v);

    // Generate output
    float cx,cy;
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    post_process(&v, &cx, &cy);
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;

    // Print output
    printf("Centroid at: %f,%f\n",cx,cy);
    printf("found in %dms\n",msec);

    return EXIT_SUCCESS;
}
