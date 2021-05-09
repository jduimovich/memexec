#include <stdio.h>
#include <sys/personality.h> 

char executable_code[128]; 
typedef void(function_call)();
 
void change_personality () {  
  long pers = personality(0xffffffffUL);
  if (pers < 0L) { 
      perror("Error getting personality:");
  }
  printf("Initial Personality == %lx\n", pers); 
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
}

int main(int argc, char *argv[]) { 
  printf("Exec Memory Tests Using personality\n"); 
  change_personality ();

  printf("Exec in static memory\n"); 
  executable_code [0] = 0xC3; // flat mode near return 
  function_call *f = (function_call *)&executable_code[0];
  (*f) ();
  printf("Done Exec in static memory\n"); 

  printf("Exec in malloc memory\n");     
  char * allocated = (char*) malloc (128);
  allocated [0] = 0xC3; // flat mode near return 
  f = (function_call *)&allocated[0];
  (*f) ();  
  printf("Done Exec in malloc memory\n");  
  
  return 0; 
}
