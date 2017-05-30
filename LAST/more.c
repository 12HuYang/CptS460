/****************more.c file ********************/
#include "ucode.c"

printpage(int fd)
{
  int i;
  putc('\n');
//  printf("at page %d\n",fd);
  for(i=0;i<24;i++)
  {
    printline(fd);
  }
}

printline(int fd)
{
  int i, n;
  char c;

  for(i=0;i<80;i++)
  {
    n=read(fd,&c,1);
    if(n)
    {
      //write(1,&c,1);
      if(fd!=0&&c=='\n')
	write(1,"\n\r",2);
      if(fd==0&&c=='\r')
      {
	write(1,"\n",2);
	//write(2,"\r",1);
      }
      write(1,&c,1);
    }
    else
      exit(0);
  }
}

main(int argc, char *argv[])
{
  int fd,i,j;
  char c,input,tty[64];

  if(argc==1)  //pipe
  {
    fd=dup(0);
    close(0);
    gettty(tty);
    open(tty,0);
  }
  else
    fd=open(argv[1],O_RDONLY);
  if(fd<0)
  {
    printf("ERROR: %s is not a file\n",argv[1]);
    exit(1);
  }
  write(2,"######Y.H more######\n\r",20);
  while(1)
  {
    //printpage(fd);
    input=getc();
    if(input==' ') printline(fd);
    else
       printpage(fd);
  //  switch(input)
  //  {
  //    case '\r': printpage(fd);
  //    case ' ': printline(fd);
  //  }
  }
}
