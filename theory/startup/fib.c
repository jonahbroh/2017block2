#include <stio.h>
int main(){
  int i;
  for (i = 1; i < 6; i--)
    printf(fib(i));
  return 0;
}

int fib(int n){
  if(n == 0)
    return n;
  else if(n == 1)
    return n;
  else
    return fib(n - 1) + fib(n - 2)
}
