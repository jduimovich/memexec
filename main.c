#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

void make_rwx(char *m, int len) { 
  // int pagesize = sysconf(_SC_PAGE_SIZE);
  // if (pagesize == -1){
  //     perror("sysconf");
  // }
  // char *buffer = memalign(pagesize, 4 * pagesize);

  int r = mprotect((void *)m,   len,    PROT_READ |  PROT_WRITE| PROT_EXEC);
  printf ("RWX %d\n", r);
}

char executable_code[128]; 
typedef void(function_call)();

/* demo uses a simple return statement  (0xC3).
To proved the code is actually running
you can use a 0xCC which is a breakpoint
the code wiil abort with Trace/breakpoint trap
*/

int main(int argc, char *argv[]) { 

  int pagesize = sysconf(_SC_PAGE_SIZE);

  printf("_SC_PAGE_SIZE %d\n", pagesize); 

  printf("Exec code in static memory\n"); 
  executable_code [0] = 0xC3; // flat mode near return 
  make_rwx(executable_code, sizeof (executable_code)) ;
  function_call *f_static = (function_call *)&executable_code[0];
  (*f_static) (); 
 
  printf("Exec code in malloc memory\n"); 
  char * allocated = (char*) malloc (128);
  make_rwx(allocated, 128) ;
  allocated [0] = 0xC3; // flat mode near return 
  function_call *f_malloc = (function_call *)&allocated[0];
  (*f_malloc) ();   

  printf("Exec code in stack memory\n"); 
  char stack[128];
  make_rwx(stack, 128) ;
  stack [0] = 0xC3; // flat mode near return 
  function_call *f_stack = (function_call *)&stack[0];
  (*f_stack) (); 

  return 0; 
}
