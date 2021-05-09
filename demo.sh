
cc -z execstack main.c -o  demo_w_exec
cc main.c -o  demo_no_exec

echo Running demo_w_exec - compiled with -z execstack 
echo Expect this to SUCCEED 
./demo_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "SUCCESS - Successfully ran executable code"
else
  echo "FAIL to run executable code despite -z execstack" 
fi

echo -n 

echo Running demo_w_exec - compiled without -z execstack 
echo Expect this to CRASH 
./demo_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "FAIL - Without -z execstack this should crash" 
else
  echo "SUCCESS - Without -z execstack this should crash"  
fi
