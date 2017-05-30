/**********************grep.c file****************************/
#include "ucode.c"

main(int argc, char *argv[])
{
    int i,n,fd,size,valid,linenum;
    char buf[256],*lp;
    if(argc>3 || argc<2)
    {
      printf("ERROR: grep pattern [f1]\n");
      exit(1);
    }
    if(argc==2)
    {
      fd=0;
    }
    else
      fd=open(argv[2],O_RDONLY);
    if(fd<0)
    {
      printf("ERROR: %s is not a file\n",argv[2]);
      exit(1);
    }
    write(2,"#############Y.H grep #############\n\r",37);
    size=strlen(argv[1]);
    n=1;
    linenum=0;
    while(n)
    {
      linenum++;
      for(i=0;i<256;i++)
      {
	n=read(fd,&buf[i],1);
	if(n<1||buf[i]=='\n')
	{
	  buf[i+1]='\0';
	  break;
	}
      }
      lp=buf;
      while(*lp!='\n')
      {
	if(!strncmp(lp,argv[1],size)&&buf[0]!=0)
	{
//	  printf("%d: %s",linenum,buf);
	  write(1,buf,(i+1));
	  putc('\r');
	  break;
	}
	lp++;
      }
      buf[0]=0;
      
    }
    close(fd);
    exit(0);

}
