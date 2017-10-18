int main(){
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <sys/wait.h>
  char* javapath = "/home/nfs/j_broh/2017block2/project/src/marioai/classes";
  char* classname = "ch.idsia.scenarios.Play";
  execl(javapath, "java", classname, "1", "999", "1111111222222233333330000000");
}
