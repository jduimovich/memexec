
cc -z execstack main.c -o  demo_w_exec
cc main.c -o  demo_no_exec

echo Running demo_w_exec - compiled with -z execstack 
echo Expect this to SUCCEED 
./demo_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "Correct - Successfully ran executable code"
else
  echo "Fail to run executable code" 
fi

echo -n 

echo Running demo_w_exec - compiled without -z execstack 
echo Expect this to CRASH 
./demo_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "Correct - No Exec Stack should crash"
else
  echo "FAIL - No Exec Stack should not succeed" 
fi
