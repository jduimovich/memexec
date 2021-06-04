#!/bin/bash

cc -m64 -z execstack main.c -o  demo_w_exec
cc -m64 main.c -o  demo_no_exec
cc -m64 personality.c -z execstack -o  demo_personality_w_exec
cc -m64 personality.c -o  demo_personality_no_exec
cc -m64 malloctest.c -o  demo_malloc_no_exec
cc -m64 mmapalloc.c -o  demo_mmapalloc_no_exec


print_result () {
    ERR=$1
    if [ $ERR -eq 0 ]
    then
	echo "YES - Successfully ran executable code"
    else
	echo "NO - Did not run executable code" 
    fi
}
echo binary execstack flags are 
execstack -q demo_* 
echo linux kernel is 
uname -r 

echo Running demo_w_exec - compiled with -z execstack
#Expect this to SUCCEED on older kernels
./demo_w_exec
print_result $?

#echo ' ' 
#echo Running demo_no_exec - compiled without -z execstack 
#Expect this to CRASH so skip it
#./demo_no_exec
#print_result $? 

echo starting ...
echo ' ' 
echo Running demo_personality_w_exec - compiled with -z execstack 
./demo_personality_w_exec malloc
print_result $?
echo ' ' 
./demo_personality_w_exec static
print_result $?
echo ' ' 
./demo_personality_w_exec stack
print_result $?
echo ' ' 
./demo_personality_no_exec brk
print_result $?

echo ' ' 
echo Running demo_personality_no_exec - compiled without -z execstack 
./demo_personality_no_exec malloc
print_result $?
echo ' ' 
./demo_personality_no_exec static
print_result $?
echo ' ' 
./demo_personality_no_exec stack
print_result $?
echo ' ' 
./demo_personality_no_exec brk
print_result $?

echo ' ' 
echo Running demo_malloc_no_exec - no execstack use mprotect to enable execute
./demo_malloc_no_exec
print_result $?

echo ' ' 
echo Running demo_mmapalloc_no_exec - no execstack use mprotect to enable execute
./demo_mmapalloc_no_exec
print_result $?

