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
 
char *align_to (char *m, int align) { 
  printf ("Align %p to %x\n", m, align); 
  long aligned = ((long)(m + align-1) & ~(align-1));  
  printf ("Align %p to %lx\n", m, aligned);
  return (char *) aligned;
}
 
typedef void(function_call)();

/* demo uses a simple return statement  (0xC3).
To proved the code is actually running
you can use a 0xCC which is a breakpoint
the code wiil abort with Trace/breakpoint trap
*/

int main(int argc, char *argv[]) { 

  int pagesize = sysconf(_SC_PAGE_SIZE);
  printf("_SC_PAGE_SIZE %d\n", pagesize); 
  if (sizeof (void*) != 8) {  
      printf("ERROR must be 64 bit %d\n", sizeof (void*)); 
      exit (-1);
  } 

  { 
    printf("Exec code in malloc memory\n"); 
    /* aligns the memory returned via mprotect skips front of allocated to align  
    */
    char * raw = (char*) malloc (pagesize*2);
    char * allocated =  align_to(raw, pagesize); 
    printf ("Trying Allocated %p\n",allocated);  
    make_rwx(allocated, pagesize) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

  {
    printf("Exec code in memalign memory\n"); 
    /* memalign and mprotect  
    */ 
    char * allocated =  (char*) memalign(pagesize, 16); 
    printf ("memalign Allocated %p\n",allocated);  
    make_rwx(allocated, pagesize) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

 

  return 0; 
}
