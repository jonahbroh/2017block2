#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  char* javapath = "/modules/packages/jdk-9/bin/java";
  char* gamepath = "/home/nfs/j_broh/2017block2/project/src/marioai/classes";
  char* classname = "ch.idsia.scenarios.Play";
  execl(javapath, javapath, "-cp", gamepath, classname, "1", "999", "1111111222222233333330000000");
}