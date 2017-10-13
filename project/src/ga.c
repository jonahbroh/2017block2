#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* javapath = "/modules/packages/jdk-9";
char* gamepath = "../Mario/";

int pop_size = 100;

struct population{
  agent[pop_size] agents;
}

struct agent{
  int fitness = 0;
  int[16] chromosome;
}

population init_population(){
  population pop;
  for(int i = 0; i < pop_size; i++){
    pop.agents[i] = generateRandomAgent();
  }
  return pop;
}

void fitness(population pop){
  for(int i = 0; i < pop_size; i++){
    switch ((pid = fork())){
      case -1:
        //OH NO
      case 0:
        //Pass i to avoid race conditions?
        execl(javapath, "-jar", gamepath, chromosome_string(pop.agents[i]));
      default:
        //Wait, check, set fitness
    }

  }

}

void sort_fitness(population pop){
  qsort(pop, pop_size, sizeof(agent), sort_func);
}

agent mutate(agent child, int mutation_rate=10){
  if(rand()%100 < mutation_rate){
    for(int i = 1; i < 16; i+=2){
      a.chromosome[i] = rand()%4;
    }
  }
  return child;
}

agent crossover(agent p1, agent p2){
  agent child;
  for(i = 1; i < 16; i+=2){
    if rand()%2 == 0{
      child.chromosome[i] = p1.chromosome[i];
    }
    else{
      child.chromosome[i] = p2.chromosome[i];
    }
  }
  return child;
}

agent pickFitParent(population pop){
  int total_fitness = sum_fitness(pop);
  int r = rand()%total_fitness;
  ind = -1;
  for(int i = 0; r > 0; i++){
    r -= pop.agents[i].fitness;
  }
  return pop.agents[i];
}

agent generateRandomAgent(){
  agent a;
  for(int i = 0; i < 16; i++){
    if((i+1)%2 == 1){
      a.chromosome[i] = i/2 + 1;
    }
    else{
      a.chromosome[i] = rand()%4;
    }
  }
  return a;
}

population new_population(population pop, int crossover_rate = 80){
  population new_pop;
  for(i = 0; i < pop_size; i++){
    int r = rand()%100;
    if(r < crossover_rate){
      agent p1 = pickFitParent(pop);
      agent p2 = pickFitParent(pop);
      agent child = crossover(p1, p2);
      child = mutate(child);
      new_pop[i] = child;
    }
    else{
      agent child = pickFitParent(pop);
      child = mutate(child);
      new_pop[i] = child;
    }
  }
  return new_pop;
}

int sum_fitness(population pop){
  int total = 0;
  for(i = 0; i < pop_size; i++){
    total += pop.agents[i].fitness;
  }
  return total;
}

int sort_func(agent e1, agent e2){
  return e1.fitness - e2.fitness;
}

char* chromosome_string(agent a){
  char chrom[100];
  int n = 0;
  for(i = 0; i < 16; i++){
    n += snprintf(&chrom[n], "%d,", a.chromosome[i]);
  }
  return chrom;
}

int main(int argc, char** argv) {
  population pop = init_population;
  for(i = 0; i < num_epochs; i++){
    fitness(pop);
    pop = new_population(pop);
  }
}
