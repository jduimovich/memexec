#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

void make_rwx(char *m, int len) {  
  int r = mprotect((void *)m,   len,    PROT_READ |  PROT_WRITE| PROT_EXEC);
  printf ("RWX %d\n", r);
}

#define PAGE_SIZE 4096 
char *align_to (char *m, int align) { 
  printf ("Align %p to %x\n", m, align);
  char *a = m + (align-1); 
  long aa = (long)a;
  aa &= ~(align-1);
  a=(char*)aa;
  printf ("Align %p to %p\n", m, a);
  return a;
}
 
typedef void(function_call)();

/* demo uses a simple return statement  (0xC3).
To proved the code is actually running
you can use a 0xCC which is a breakpoint
the code wiil abort with Trace/breakpoint trap
*/

int main(int argc, char *argv[]) { 

  int pagesize = sysconf(_SC_PAGE_SIZE);
  if (pagesize != PAGE_SIZE) {  
      printf("ERROR on PAGE_SIZE assumptions %d\n", pagesize); 
      exit (-1);
  }

  printf("_SC_PAGE_SIZE %d\n", pagesize); 
 
  printf("Exec code in malloc memory\n"); 
  char * raw = (char*) malloc (PAGE_SIZE*2);
  char * allocated =  align_to(raw, PAGE_SIZE);

  printf ("Trying Allocated %p\n",allocated);

  make_rwx(allocated, PAGE_SIZE) ;
  allocated [0] = 0xC3; // flat mode near return 
  function_call *f_malloc = (function_call *)&allocated[0];
  (*f_malloc) ();   
 

  return 0; 
}
