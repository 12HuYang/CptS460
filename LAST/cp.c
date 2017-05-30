/****************cp.c*************************/
#include "ucode.c"

main(int argc, char *argv[])
{
   char buf[1024];
   int i,n,fd1,fd2;
   printf("######## Y.H cp ########\n");
    if(argc!=3)
    {
       printf("ERROR: cp f1 f2\n");
       exit(1);
    }
    creat(argv[2]);
    fd1=open(argv[1],O_RDONLY);
    if(fd1<0)
    {
        printf("ERROR: %s not a file\n",argv[1]);
        exit(1);
    }
    fd2=open(argv[2],O_WRONLY);
    while(n=read(fd1,buf,1024))
    {
       write(fd2,buf,n);
    }
    close(fd1);
    close(fd2);
    exit(0);
}

