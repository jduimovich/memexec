
cc -z execstack main.c -o  demo_w_exec
cc main.c -o  demo_no_exec

cc personality.c -z execstack -o  demo_personality_w_exec
cc personality.c -o  demo_personality_no_exec

echo Running demo_w_exec - compiled with -z execstack 
#Expect this to SUCCEED 
./demo_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "SUCCESS - Successfully ran executable code"
else
  echo "FAIL to run executable code despite -z execstack" 
fi

echo ' ' 
echo Running demo_w_exec - compiled without -z execstack 
#Expect this to CRASH 
./demo_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then
  echo "FAIL - Without -z execstack this should crash but did not" 
else
  echo "SUCCESS - Without -z execstack this should crash"  
fi

echo ' ' 
echo Running demo_personality_w_exec - compiled with -z execstack 
./demo_personality_w_exec
ERR=$?
if [ $ERR -eq 0 ]
then 
  echo "SUCCESS - with -z execstack should succeed"  
else
  echo "FAILED - with -z execstack should succeed"  
fi

echo ' ' 
echo Running demo_personality_w_exec - compiled with -z execstack 
./demo_personality_no_exec
ERR=$?
if [ $ERR -eq 0 ]
then 
  echo "SUCCESS - without -z execstack this should now succeed"  
else
  echo "FAILED - without -z execstack this should now succeed but did not"   
fi 