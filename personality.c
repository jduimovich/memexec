#include <stdio.h>
#include <stdlib.h> 
#include <sys/personality.h> 

char executable_code[128]; 
typedef void(function_call)();
 
void change_personality () {  
  long pers = personality(0xffffffffUL);
  if (pers < 0L) { 
      perror("Error getting personality:");
  }
  printf("Initial Personality == %lx\n", pers); 
  if (pers & ADDR_NO_RANDOMIZE) { 
    printf("ADDR_NO_RANDOMIZE is already set\n");  
  }
  if (pers & READ_IMPLIES_EXEC) { 
    printf("READ_IMPLIES_EXEC is already set\n");  
  }
  long new_pers = personality(pers | ADDR_NO_RANDOMIZE | READ_IMPLIES_EXEC);
  if (new_pers < 0L) { 
      perror("Error adding ADDR_NO_RANDOMIZE and READ_IMPLIES_EXEC");
  }
  printf("Ret value for ADDR_NO_RANDOMIZE and READ_IMPLIES_EXEC  == %lx\n", new_pers); 
  pers = personality(0xffffffffUL);
  if (pers < 0L) { 
      perror("Error getting personality:");
  } 
  printf("New Personality == %lx\n", pers); 
    if (pers & ADDR_NO_RANDOMIZE) { 
    printf("ADDR_NO_RANDOMIZE is  set\n");  
  }
  if (pers & READ_IMPLIES_EXEC) { 
    printf("READ_IMPLIES_EXEC is  set\n");  
  }

}

int main(int argc, char *argv[]) { 
  printf("Exec Memory Tests using personality\n"); 
  change_personality ();

  printf("Exec code in static memory\n"); 
  executable_code [0] = 0xC3; // flat mode near return 
  function_call *f_static = (function_call *)&executable_code[0];
  (*f_static) (); 
 
  printf("Exec code in malloc memory\n"); 
  char * allocated = (char*) malloc (128);
  allocated [0] = 0xC3; // flat mode near return 
  function_call *f_malloc = (function_call *)&allocated[0];
  (*f_malloc) ();   

  printf("Exec code in stack memory\n"); 
  char stack[128];
  stack [0] = 0xC3; // flat mode near return 
  function_call *f_stack = (function_call *)&stack[0];
  (*f_stack) (); 

  return 0; 
}
