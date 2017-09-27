#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    size_t T=8;
    int t_hello[T];
    int i=0;
    for(i=0;i<T;i++) {
        t_hello[i]=-1;
    }
    i=0;
    printf("%d\n", i);
    #pragma omp parallel num_threads(8)
    {
          t_hello[i]=omp_get_thread_num();
          i++;
          printf("%d\n", i);
          while (i < T){
            printf("%d\n", i);
            sleep(1);
          }
    }

    for(i=0;i<T;i++) {
        printf("Hello from %d\n",t_hello[i]);
    }

    return 0;
}
