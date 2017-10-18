#include <stdio.h>
int main()
{
  for(int i = 0; i < 100; i++){
    char fitpath[100];
    char* fitdir = "/home/nfs/j_broh/2017block2/project/src/marioai/scores/fitness";
    sprintf(fitpath, "%s%d.txt", fitdir, i);
    FILE* file_ptr = fopen(fitpath, "w");
    fclose(file_ptr);
  }
  return 0;
}
