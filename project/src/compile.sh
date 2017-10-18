#! /bin/bash
gcc -o maketxt test.c

# Compile the serial version
gcc -o ga_serial ga.c

# Compile the OpenMP version
gcc -o ga_openmp -fopenmp ga_openmp.c
