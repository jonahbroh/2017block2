Run compile.sh to compile. Run maketxt to create empty text files for the Java to write to, run ga_serial for the serial version, run ga_openmp for the openmp version.
Population size can be changed at the top of both ga.c and ga_openmp.c, anything beyond 10 in the serial and 20 or 30 in the parallel takes prohibitively long.
To change variables in the Java code, modify the files below(in marioai/src) and either manually compile them in src and copy them to classes or use Apache Ant or something similar to compile using the build.xml file.
Difficulty: line 36 in ch/idsia/scenarios/Play.java, low easier, high more difficult(more time for the GA to reach an idea fitness). Anything higher than 2 or 3 takes prohibitively long in the serial version.
Time: line 203 in ch/idsia/utils/ParameterContainer.java, affects maximum time taken per evaluation, especially noticeable in early generations
To run the Java outside the genetic algorithm, from marioai/classes, java ch.idsia.scenarios.Play 1 [OUTPUTNUM] [CHROMOSOME] where OUTPUTNUM is the number appended to the name of the fitness.txt file output is saved to and CHROMOSOME is a 28-digit number generated by the GA.
