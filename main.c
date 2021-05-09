#include <stdio.h>
#include <stdlib.h> 

char executable_code[128]; 
typedef void(function_call)();

int main(int argc, char *argv[]) { 
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
