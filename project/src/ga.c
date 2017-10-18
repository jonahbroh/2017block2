//Sort function,

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


char* javapath = "/modules/packages/jdk-9/bin/java";
char* gamepath = "/home/nfs/j_broh/2017block2/project/src/marioai/classes";
char* classname = "ch.idsia.scenarios.Play";

int pop_size = 10;
int mutation_rate = 10;
int crossover_rate = 80;
int num_epochs = 500;

typedef struct{
  int fitness;
  int* chromosome;
} agent;

typedef struct{
  agent* agents;
} population;


int sort_func(const void *e1, const void *e2){
  return ((agent *)e1)->fitness - ((agent *)e2)->fitness;
}

void sort_fitness(population pop){
  qsort(pop.agents, pop_size, sizeof(agent), sort_func);
}

char* chromosome_string(agent a){
  char* chrom = malloc(100 * sizeof(char));
  int n = 0;
  for(int i = 0; i < 28; i++){
    n += sprintf(&chrom[n], "%d", a.chromosome[i]);
  }
  return chrom;
}
agent generateRandomAgent(){
  agent a;
  a.chromosome = malloc(28 * sizeof(int));
  for(int i = 0; i < 28; i++){
    a.chromosome[i] = rand()%4;
  }
  a.fitness = 0;
  return a;
}

population init_population(){
  printf("!\n");
  population pop;
  pop.agents = malloc(pop_size * sizeof(agent));
  for(int i = 0; i < pop_size; i++){
    pop.agents[i] = generateRandomAgent();
  }
  return pop;
}

void fitness(population pop){
  pid_t parent, pid;
  for(int i = 0; i < pop_size; i++){
    int status;
    parent = getpid();
    pid = vfork();
    if (pid == -1){
      printf("!\n");
    }
    else if (pid == 0){
      char istr[5];
      sprintf(istr, "%d", i);
      execl(javapath, javapath, "-cp", gamepath, classname, "1", istr, chromosome_string(pop.agents[i]), (char*)NULL);
      _exit(0);
    }
    // waitpid(pid, NULL, 0);
    wait(NULL);
    char fitpath[100];
    char* fitdir = "/home/nfs/j_broh/2017block2/project/src/marioai/scores/fitness";
    char fitstr[1000];
    sprintf(fitpath, "%s%d.txt", fitdir, i);
    FILE *fit = fopen(fitpath, "r");
    fgets(fitstr, 1000, fit);
    printf("%d %s\n", i, fitstr);
    pop.agents[i].fitness = atoi(fitstr);

  }
  sort_fitness(pop);
}

agent mutate(agent child, int mutation_rate){
  if(rand()%100 < mutation_rate){
    for(int i = 0; i < 28; i++){
      child.chromosome[i] = rand()%4;
    }
  }
  return child;
}

agent crossover(agent p1, agent p2){
  agent child;
  child.chromosome = malloc(28 * sizeof(int));
  for(int i= 1; i < 28; i+=2){
    if(rand()%2 == 0){
      child.chromosome[i] = p1.chromosome[i];
    }
    else{
      child.chromosome[i] = p2.chromosome[i];
    }
  }
  return child;
}

int sum_fitness(population pop){
  int total = 0;
  for(int i= 0; i < pop_size; i++){
    total += pop.agents[i].fitness;
  }
  return total;
}

agent pickFitParent(population pop){
  printf("?\n");
  int total_fitness = sum_fitness(pop);
  int r;
  if(total_fitness > 0){
    r = rand()%total_fitness;
  }
  else{
    r = 0;
  }
  int ind = 0;
  for(int i = 0; r > 0 && i < pop_size; i++){
    ind = i;
    r -= pop.agents[i].fitness;
  }
  return pop.agents[ind];
}

population new_population(population pop, int crossover_rate){
  population new_pop;
  new_pop.agents = malloc(pop_size * sizeof(agent));
  for(int i= 0; i < pop_size; i++){
    int r = rand()%100;
    printf("%di\n", i);
    if(r < crossover_rate){
      agent p1 = pickFitParent(pop);
      agent p2 = pickFitParent(pop);
      agent child = crossover(p1, p2);
      child = mutate(child, mutation_rate);
      new_pop.agents[i] = child;
    }
    else{
      agent child = pickFitParent(pop);
      child = mutate(child, mutation_rate);
      new_pop.agents[i] = child;
    }
  }
  return new_pop;
}

int main(int argc, char** argv) {
  population pop = init_population();
  for(int i= 0; i < num_epochs; i++){
    fitness(pop);
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC,&end_time);
    long msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
    printf("Generation %d, top fitness %d, %d msec\n", i, pop.agents[pop_size - 1].fitness, msec);
    if(pop.agents[0].fitness >= 4000){
      printf("Fitness: %d Generations: %d", pop.agents[0].fitness, i);
      return i;
    }
    pop = new_population(pop, crossover_rate);
  }
  printf("Fitness: %d Generations: %d", pop.agents[0].fitness, 500);
  return 500;
}
