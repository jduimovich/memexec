#include <stdio.h>
#include <sys/personality.h> 

char executable_code[128]; 
typedef void(function_call)();

int main(int argc, char *argv[]) { 
  printf("Exec Memory Tests Using personality\n"); 
  long pers = personality(0xffffffffUL);
  printf("Personality == %lx\n", pers); 
  long new_pers = personality(pers | ADDR_NO_RANDOMIZE | READ_IMPLIES_EXEC);
  

  pers = personality(0xffffffffUL);
  printf("Personality now == %lx\n", pers);    

  executable_code [0] = 0xC3; // flat mode near return 
  function_call *f = (function_call *)&executable_code[0];
  (*f) ();
  printf("Done Exec Memory Tests\n"); 
  return 0; 
}
