#include <omp.h>
#include <stdio.h>
#include <unistd.h>

omp_lock_t lockA;
int main(int argc, char** argv) {
    size_t T=8;
    int t_hello[T];
    int i=0;
    for(i=0;i<T;i++) {
        t_hello[i]=-1;
    }
    i=0;
    #pragma omp parallel num_threads(8)
    {
          omp_init_lock(&lockA);
          omp_set_lock(&lockA);
          t_hello[i]=omp_get_thread_num();
          i++;
          omp_unset_lock(&lockA);
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
