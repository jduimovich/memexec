
cc -m64 mmapalloc.c -o  demo_mmapalloc_no_exec

echo binary execstack flags are 
execstack -q demo_* 

echo ' ' 
echo Running demo_mmapalloc_no_exec - no execstack use mprotect to enable execute
./demo_mmapalloc_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then 
  echo "SUCCESS - mprotect execute of mmapped memory worked"  
else
  echo "FAILED - mprotect execute of mmapped memory failed to work"   
fi 