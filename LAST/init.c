/***************init.c file********************/
#include "ucode.c"
int console0,console1,console2;

main()
{
  int in,out,i; //file description
  in = open("/dev/tty0",O_RDONLY);
  out = open("/dev/tty0",O_WRONLY);  //for display to console
  printf("Y.H INIT: fork a login proc on port 0\n");
  console0 = fork();
  if(console0)
    {
      printf("Y.H INIT: fork a login proc on port 1\n");
      console1=fork();
      if(console1)
      {
	printf("Y.H INIT: fork a login proc on port 2\n");
	console2=fork();
        if(console2)
	{
	  parent();
	}
	else
	  exec("login /dev/ttyS1");
      }
      else
      {
	exec("login /dev/ttyS0");
      }
    } 
  else
    exec("login /dev/tty0");
}

int parent()
{
  int pid,status;
  while(1)
  {
    printf("Y.H INIT: wait for ZOMBIE child\n");getc();
    pid=wait(&status);
    if(pid==console0||pid==console1||pid==console2)
    {
      if(pid==console0)
      {
	printf("Y.H INIT: fork a new proc for port 0\n");
	console0=fork();
	if(!console0)
	  exec("login /dev/tty0");
      }
      if(pid==console1)
      {
	printf("Y.H INIT: fork a new proc for port 1\n");
	console1=fork();
	if(!console1)
	  exec("login /dev/ttyS0");
      }
      if(pid==console2)
      {
	printf("Y.H INIT: fork a new proc for port 2\n");
	console2=fork();
	if(!console2)
	  exec("login /dev/ttyS1");
      }
    }
    else
      printf("Y.H INIT:just buried an orphan child proc %d\n",pid);
  }
}
