/***************login.c file **********************************/
/*login.c :  Upon entry, argv[0]=login, argv[1]=/dev/ttyX******/
#include "ucode.c"

int in; out; err;

main(int argc,char* argv[])
{
  char *file,name[64],passwd[64],fbuf[1024],*token,tokbuf[7][64],*tpr,shpath;
  int pwf,fdsize,i,uid,gid;
  STAT st;
  //(1) close file descriptors 0,1 inherited from INIT.
  close(0);
  close(1);
  file=argv[1];
  //(2) open argv[1] 3 times as in(0),out(1),err(2).
  in = open(file,0);
  out = open(file,1);
  err = open(file,2);
  //(3) set tty (argv[1]); //set tty name string in proc.tty
  settty(file);
  //(4) open /etc/passwd file for READ;
  //pwf=open("/etc/passwd",0);
  while(1)
  {
    printf("YH login:");gets(name);
    printf("YH password:");gets(passwd);
  //(5) for each line in passwd file do tokenize account line;
    pwf=open("/etc/passwd",0);
    stat("/etc/passwd",&st);  //get stat of passwd file
    fdsize=st.st_size;  //get file size
    //printf("pwf file size is %d\n",fdsize);getc();
    while(fdsize>0)
    {
      for(i=0;i<1024 && fdsize > 0;i++)
      {
	read(pwf,&fbuf[i],1);
//	printf("i=%d, c=%c\n",i,fbuf[i]); getc();
	fdsize--;
	if(fbuf[i]=='\n' || fbuf[i]==EOF)
	{
	  fbuf[i+1]='\0';
	  break;
	}
      }
      //printf("fdsize is %d\n",fdsize);
  //    printf("fbuf is %s\n",fbuf);
      token=strtok(fbuf,":");
      i=0;
      while(token!=NULL&&i<7)
      {
	tpr=tokbuf[i];
//	printf(" token is %s\n",token);
        strcpy(tpr,token);
	tpr[strlen(tpr)]='\0';
//	printf("tokbuf [%d]: %s\n",i,tokbuf[i]);
	token=strtok(NULL,":");
	i++;
      }
  //(6) if user has a valid account
      printf("name is %s, passwd is %s\n",name,passwd);
      //printf("%d,%d",strcmp(name,tokbuf[0]),strcmp(passwd,tokbuf[1]));
      getc();
      if(!strcmp(name,tokbuf[0])&&!strcmp(passwd,tokbuf[1]))
      {
  //(7) change uid,gid to user's uid,gid;  chuid()
        uid=atoi(tokbuf[2]);
	gid=atoi(tokbuf[3]);
	printf("uid= %d, gid=%d",uid,gid);
	getc();
      //change cwd to user's homoe DIR; chdir()
        chdir(tokbuf[5]);
      //close opened /etc/passwd file; close()
	close(pwf);
      //exec to program in user account; exec()
      //exec("/.../sh");
	exec("/bin/sh");
      }
    }

      //close opened /etc/passwd file; close()
      close(pwf);
      //exec to program in user account; exec()
      //exec("/.../sh");

    printf("login failed, try again \n");
  }
}
