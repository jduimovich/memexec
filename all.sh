
cc -m64 -z execstack main.c -o  demo_w_exec
cc -m64 main.c -o  demo_no_exec
cc -m64 personality.c -z execstack -o  demo_personality_w_exec
cc -m64 personality.c -o  demo_personality_no_exec
cc -m64 malloctest.c -o  demo_malloc_no_exec
cc -m64 mmapalloc.c -o  demo_mmapalloc_no_exec

echo binary execstack flags are 
execstack -q demo_* 
echo linux kernel is 
uname -r 

echo Running demo_w_exec - compiled with -z execstack
#Expect this to SUCCEED 
./demo_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
    echo "NO - Did not run executable code" 
fi

echo ' ' 
echo Running demo_no_exec - compiled without -z execstack 
#Expect this to CRASH 
./demo_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
  echo "NO - Did not run executable code" 
fi

echo ' ' 
echo Running demo_personality_w_exec - compiled with -z execstack 
./demo_personality_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
    echo "NO - Did not run executable code" 
fi

echo ' ' 
echo Running demo_personality_no_exec - compiled with -z execstack 
./demo_personality_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
    echo "NO - Did not run executable code" 
fi

echo ' ' 
echo Running demo_malloc_no_exec - no execstack use mprotect to enable execute
./demo_malloc_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
    echo "NO - Did not run executable code" 
fi


echo ' ' 
echo Running demo_mmapalloc_no_exec - no execstack use mprotect to enable execute
./demo_mmapalloc_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "YES - Successfully ran executable code"
else
    echo "NO - Did not run executable code" 
fi

