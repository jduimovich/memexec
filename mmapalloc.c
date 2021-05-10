#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

void *anon_alloc(size_t len)
{
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_PRIVATE;
    void *p = mmap(0, len, prot, flags, -1, 0);
    return p != MAP_FAILED ? p : 0;
} 
void anon_free(void *p, size_t len)
{
    munmap(p, len);
}

int make_rwx(char *m, int len)
{
    int result = mprotect((void *)m, len, PROT_READ | PROT_WRITE | PROT_EXEC);
    printf("RWX from %p for %d bytes, result = %d\n", m, len, result);
    return result;
}

typedef void(function_call)();

/* demo uses a simple return statement  (0xC3).
To proved the code is actually running you can use a 0xCC which is a breakpoint
the code wiil abort with Trace/breakpoint trap
*/

#define ALLOCATE_SIZE 4096

int main(int argc, char *argv[])
{
    int pagesize = sysconf(_SC_PAGE_SIZE);
    printf("_SC_PAGE_SIZE %d\n", pagesize);
    if (sizeof(void *) != 8)
    {
        printf("ERROR must be 64 bit %ld\n", sizeof(void *));
        exit(-1);
    }
    {
        printf("Exec code in anon_alloc \n"); 
        char *allocated = (char *)anon_alloc(ALLOCATE_SIZE);
        printf("anon_alloc allocated %p\n", allocated);
        make_rwx(allocated, ALLOCATE_SIZE);
        allocated[0] = 0xC3; // flat mode near return
        function_call *f_malloc = (function_call *)&allocated[0];
        (*f_malloc)();
    } 
    return 0;
}
