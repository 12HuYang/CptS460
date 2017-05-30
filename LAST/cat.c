/******************cat.c file***********************/
#include "ucode.c"

main(int argc, char *argv[])
{
  int i,fd=0,n,redir=2,fd2=0;
  char buf[1024],pbuf[256],tty[256],input[1],c;
  STAT st_tty,st0,st1;
  
  write(2,"############Y.H CAT##############\n\r",36);
  
  if(argc>2)
  {
    printf("ERROR input : cat filename\n");
    exit(1);
  }
  gettty(tty);
  stat(tty,&st_tty);fstat(0,&st0);fstat(1,&st1);
  if(argc==1)  //no filename, get input from stdin (fd=0)
   {
     fd=open(tty,O_RDONLY);
     if(st0.st_mode & 0010000)  // | cat |
     {
      //  write(2,"pipe\n\r",6);
        while(read(0,input,1))
	{
	  //write(1,buf,n);
          //printf(buf);
          input[0]=input[0]+'\0';
//          write(2,input,1);
          write(1,input,1);
	}
     }else if(st0.st_mode & 0100000)  // cat < file
     {
       // write(2,"r0\n\r",4);
        while(n=read(0,buf,1024)){
              buf[n]='\0';
	       for(i=0;i<n;i++)
	       {
		  switch(buf[i])
		  {
		     case '\n':putc('\n');putc('\r');break;
		     default  :putc(buf[i]);break;
		  }
	       }
       
      }
     }else if(st1.st_mode & 0100000)  //cat > file
     {
      //  write(2,"r1\n\r",4);
	while(1)
	{
	  c=getc();
          input[0]=c+'\0';
	  switch(c)
	  {
	    case '\r':write(2,"\n",1);write(2,"\r",1);putc('\n');break;
	    case '~' :exit(0);
            default  :write(2,input,1);putc(c);break;
	  }
          
	}
	//  write(1,input,1);
     }else             
     {
       // write(2,"12\n\r",4);
        while(1)
	{
	  c=getc();
	  switch(c)
	  {
	    case '\r': putc('\n');break;
	    case '~' : exit(0);
	    default  : putc(c);break;
	  }
	}
     }
   }
  if(argc==2)  //check redirect or not
  {
    fd=open(argv[1],O_RDONLY);
    if(fd<0)
    {
        printf("ERROR cannot open file %s",argv[1]);
        exit(1);
    } 
    while(n=read(fd,buf,256))
    {
      buf[n]='\0';
      i=0;
     // if(st1.st_mode & 0010000)
    // {
    //    write(2,"w4",2);
        //write(2,buf,n);
    //    write(1,buf,n);
    // }
      if(st1.st_mode & 0100000) //cat f1 > f2
      {
    //    write(2,"w1",2);
        write(1,buf,n);
        //putc(buf[i]);
        //i++;
      }
      else if(st0.st_mode & 0100000) //cat f1 < f2
      {
  //      write(2,"w0",2);
        while(buf[i])
	{
	  if(buf[i]=='\n') {putc('\n');putc('\r');}
	  else{
	    putc(buf[i]);}
	  i++;
	}
        //write(1,buf,n);
      }
      else  //cat f1
      {
//         write(2,"w3",2);
         while(buf[i])
         {
            switch(buf[i])
            {
               case '\n': putc('\n');putc('\r');break;
               default  : putc(buf[i]);
            }
            i++;
         }
      } 
    }
    close(fd);
    write(2,"\n\r",2);
  }
  exit(0);
}

