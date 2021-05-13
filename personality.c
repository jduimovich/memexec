#include <stdio.h>
#include <stdlib.h> 
#include <sys/personality.h> 
#include <string.h>
#include <unistd.h>

char executable_code[128]; 
typedef void(function_call)();
 
void change_personality () {  
  int pers = personality(0xffffffffUL);
  if (pers < 0L) { 
      perror("Error getting personality:");
  }
   printf("Initial Personality == %x ASR %s, READ_IMPLIES_EXEC %s\n", pers,
	  ((pers & ADDR_NO_RANDOMIZE) ? "yes" : "no"),
	  ((pers & READ_IMPLIES_EXEC) ? "yes" : "no")
	  ); 

  int new_pers = personality(pers | ADDR_NO_RANDOMIZE | READ_IMPLIES_EXEC);
  //  int new_pers = personality(pers | READ_IMPLIES_EXEC);
  if (new_pers < 0L) { 
      perror("Error adding ADDR_NO_RANDOMIZE and READ_IMPLIES_EXEC");
  }
  pers = personality(0xffffffffUL);
  if (pers < 0L) { 
      perror("Error getting personality:");
  } 
  printf("New Personality == %x ASR %s, READ_IMPLIES_EXEC %s\n", pers,
	  ((pers & ADDR_NO_RANDOMIZE) ? "yes" : "no"),
	  ((pers & READ_IMPLIES_EXEC) ? "yes" : "no")
	  ); 
}

int main(int argc, char *argv[]) { 
  printf("Exec Memory Tests using personality\n"); 
  change_personality ();
  int do_malloc=0;
  int do_static=0;
  int do_stack=0;
  int do_brk=0;

  if (argc==1) {
    printf ("Running all tests\n"); 
    do_malloc=1;
    do_static=1;
    do_stack=1;
    do_brk=1;
  } else { 
    for (int i=1;i<argc;i++) {
      if (!strcmp (argv[i], "malloc")) do_malloc=1;
      if (!strcmp (argv[i], "static")) do_static=1;
      if (!strcmp (argv[i], "stack")) do_stack=1;
      if (!strcmp (argv[i], "brk")) do_brk=1;
    }
  }

  if(do_static) { 
    printf("Exec code in static memory\n"); 
    executable_code [0] = 0xC3; // flat mode near return 
    function_call *f_static = (function_call *)&executable_code[0];
    (*f_static) ();
  }

  if (do_malloc) {
     printf("Exec code in malloc memory\n"); 
     char * allocated = (char*) malloc (128);
     allocated [0] = 0xC3; // flat mode near return 
     function_call *f_malloc = (function_call *)&allocated[0];
     (*f_malloc) ();
  }

  if (do_brk) {
     printf("Exec code allocated by brk and sbrk memory\n"); 
     char * allocated = (char*) sbrk(4096);
     allocated [0] = 0xC3; // flat mode near return 
     function_call *f_malloc = (function_call *)&allocated[0];
     (*f_malloc) ();
  }

  if (do_stack) { 
    printf("Exec code in stack memory\n"); 
    char stack[128];
    stack [0] = 0xC3; // flat mode near return 
    function_call *f_stack = (function_call *)&stack[0];
    (*f_stack) ();
  }

  return 0; 
}
