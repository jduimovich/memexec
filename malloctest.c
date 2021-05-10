#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 
#include <sys/mman.h>

int make_rwx(char *m, int len) {  
  int result = mprotect((void *)m, len, PROT_READ |  PROT_WRITE| PROT_EXEC);
  printf ("RWX %d\n", result);
  return result;
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
      printf("ERROR must be 64 bit %ld\n", sizeof (void*)); 
      exit (-1);
  } 

  { 
    printf("Exec code in malloc memory\n"); 
    /* aligns the memory returned via mprotect skips front of allocated to align 
       over allocate the memory to allow for alignment (need an extra pagesize-1 bytes)
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
    printf("Exec code in malloc memory\n"); 
    /* Hack version, align the pages around the allocated pointer
       Do not change the pointer, just adjust for alignment 
    */
    char * allocated = (char*) malloc (pagesize*2);
    long aligned = (((long)allocated) & ~(pagesize-1));  
    char * protect =  (char*) aligned;  
    // add the rounded down to length
    make_rwx(protect, (allocated - protect)+pagesize*2) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

  {
    printf("Exec code in memalign memory\n"); 
    /* memalign and mprotect  
    */ 
    char * allocated =  (char*) aligned_alloc(pagesize, pagesize); 
    printf ("memalign Allocated %p\n",allocated);  
    make_rwx(allocated, pagesize) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }
 
  return 0; 
}
