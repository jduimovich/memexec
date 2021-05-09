
cc -z execstack main.c -o  demo_w_exec
cc main.c -o  demo_no_exec

echo Running demo_w_exec - compiled with -z execstack 
echo Expect this to SUCCEED 
./demo_w_exec

echo Running demo_w_exec - compiled without -z execstack 
echo Expect this to CRASH 
./demo_no_exec
