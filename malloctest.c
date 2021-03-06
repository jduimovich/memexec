#include <unistd.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 
#include <sys/mman.h>

int make_rwx(char *m, int len) {  
  int result = mprotect((void *)m, len, PROT_READ |  PROT_WRITE| PROT_EXEC);
  printf ("RWX from %p for %d bytes, result = %d\n", m, len, result);
  return result;
}

/* chop off the low bits to align */
char *align_down_to (char *m, int align) { 
  long aligned = (((long)m) & ~(align-1));    
  return (char *) aligned;
}
/* add align-1 and then round down to get to next alignment boundry*/
char *align_up_to (char *m, int align) {  
  return align_down_to(m+align-1, align);  
}
 
typedef void(function_call)();

/* demo uses a simple return statement  (0xC3).
To proved the code is actually running
you can use a 0xCC which is a breakpoint
the code wiil abort with Trace/breakpoint trap
*/

#define ALLOCATE_SIZE 4096

int main(int argc, char *argv[]) {  
  int pagesize = sysconf(_SC_PAGE_SIZE);
  printf("_SC_PAGE_SIZE %d\n", pagesize); 
  if (sizeof (void*) != 8) {  
      printf("ERROR must be 64 bit %ld\n", sizeof (void*)); 
      exit (-1);
  } 

  { 
    printf("Exec code in malloc memory, mprotect interal to allocation\n"); 
    /* aligns the memory returned via mprotect skips front of allocated to align 
       over allocate the memory to allow for alignment (need an extra pagesize-1 bytes)
    */
    char * raw = (char*) malloc (ALLOCATE_SIZE+(pagesize-1));
    char * allocated =  align_up_to(raw, pagesize); 
    printf ("Allocated %p, round up to alignment %p\n",raw, allocated);  
    make_rwx(allocated, ALLOCATE_SIZE+(pagesize-1)-(allocated - raw)) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

  { 
    printf("Exec code in malloc memory, mprotect surrounding pages\n"); 
    /* Hack version, align the pages around the allocated pointer
       Do not change the pointer, just force adjust for mprotect
       USE the ORIGINAL pointer for execution 
       This hack may be problematic as it covers memory that is not part of your allocation
       There will be a memory page there but technically not to mess with. 
    */
    char * allocated = (char*) malloc (ALLOCATE_SIZE);
    long aligned = (((long)allocated) & ~(pagesize-1));  
    char * protect =  (char*) aligned;  
    printf ("Allocated %p, round down to alignment %p\n", allocated, protect);  
    // add the rounded down to allocate size to cover full range
    make_rwx(protect, (allocated - protect)+ALLOCATE_SIZE) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

  {
    printf("Exec code in aligned_alloc memory protect exact allocate size\n"); 
    /* aligned_alloc and mprotect  */ 
    char * allocated =  (char*) aligned_alloc(pagesize, ALLOCATE_SIZE); 
    printf ("aligned_alloc Allocated %p\n",allocated);  
    make_rwx(allocated, ALLOCATE_SIZE) ;
    allocated [0] = 0xC3; // flat mode near return 
    function_call *f_malloc = (function_call *)&allocated[0];
    (*f_malloc) ();   
  }

  return 0; 
}
