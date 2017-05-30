/*************************sh.c file********************************/
#include "ucode.c"


int do_pipe(char *cmdline)//, int *pd)
{
  char head[128],tail[128],*token,temp[128];
  int hasPipe,pid,pd[2];
  //if(pd) //if has a pipe passed in, as Writer on pipe pd;
 // {
 //   printf("PROC %d has pd\n",getpid());
 //   close(pd[0]);dup2(pd[1],1);close(pd[1]); //dup fd1 to fd2
 // }
  //divide cmdline into head, tail by rightmost pipe symbol
  strcpy(temp,cmdline);

  hasPipe = scan(temp,&head,&tail);
  if(hasPipe)
  {
    printf("Has pipe, head is %s, tail is %s\n",head,tail);
    pipe(pd);
    pid=fork();
    printf("child pid is %d\n",pid);
    if(pid)  //parent
    {
      //as READER  on lpd;
      printf("parent %d will be the READER\n",getpid());
      close(pd[1]);dup2(pd[0],0);close(pd[0]);
      printf("going to do_command on %s\n",tail);
      do_command(tail);
    }
    else{
      printf("child %d as WRITER on lpd\n", getpid());
      close(pd[0]);dup2(pd[1],1);close(pd[1]);
      printf("child %d do_pipe on %s\n",getpid(),head);
      do_pipe(head);
    }
  }
  else
  {
    printf("going to do_command on %s\n",cmdline);
    do_command(cmdline);
  }
}

//int scanr(char cmdline[],char *head,char *mid,char *tail, char *ch)
int scanr(char cmdline[],char *head,char *tail, char *ch)
{
  char cpy[128],cpy2[128],*p,*m;
  int i=0,j=0,k=0;
  printf("ch is %s\n",ch);
  strcpy(cpy,cmdline);
  strcpy(cpy2,cmdline);
  
  
  //p=strchr(cpy,ch);
  p=strtok(cpy,ch);
  printf("p is %s",p);
  //h=&cpy;
  if(!strcmp(ch,">>"))
  {
    m=strtok(cpy2,ch);
    m=strtok(NULL, " ");
    m=strtok(NULL, " ");
  }else{
  m=strtok(cpy2,ch);
  m=strtok(NULL," ");
  }
  //printf("h is %s\n",h);
  printf("m is %s\n",m);

 // while(p)
 // {
    while(*m)
    {
      tail[k]=*m;
      m++;
      k++;
    }
    tail[k]='\0';
    //while(h!=p)
   // {
   //   head[i]=*h;
    while(*p)
    {
      head[i]=*p;
      p++;
      i++;
    }
    head[i]='\0';

  //  p=p+2;
  //  while(*p)
  //  {
  //    tail[j]=*p;
  //    p++;
  //    j++;
  //  }
  //  tail[j]='\0';
    printf("Head is %s tail is %s\n",head,tail);
    if(!*tail)
    {
      printf("no tail\n");
      return 0;
    }
    printf("has tail\n");
    return 1;
 // }
  //return 0;
}

int do_command(char *cmdline)
{
  char head[128],tail[128],mid[128], *check, *myargv[128], *path,lsym[2],*sym;
  int ch=0, hasre,i=0,len,index=0;
  STAT *vfile;
  //scan cmdline for I/O redirection symbols;
  lsym[0]=NULL;
  sym=&lsym;
  check=strstr(cmdline,">");
  if(check) ch=1;
  check=strstr(cmdline,"<");
  if(check) ch=2;
  check=strstr(cmdline,">>");
  if(check) ch=3;
  //do I/O redirections;
  switch(ch){
    case 1: strcpy(lsym,">"); break;
    case 2: strcpy(lsym,"<"); break;
    case 3: strcpy(lsym,">>"); break;
  }
  printf("ch is %d lsym is %s sym is %s\n",ch,lsym,sym);
  if(ch!=0)
    hasre=scanr(cmdline,&head,&tail,sym);
  if(hasre)
  {
  //head=cmdLine BEFORE redirections;
    printf("PROC %d redirect output to %s\n",getpid(),tail);
    stat(tail,vfile);
    cmdline=&head;
    switch(ch){
      case 1: {                   
	printf("do >\n");
	close(1);
        creat(tail);
	open(tail,O_WRONLY);
	break;
	}
      case 2: {
      printf("do <\n");
      close(0); 
      open(tail,O_RDONLY);
      break;}
      case 3: {
      printf("do >>\n");
      close(1); 
      open(tail,O_APPEND|O_WRONLY|O_CREAT);
      break;}
    }
  }else
  {
    printf("No redirect\n");
  }
  while(cmdline[i]==' ') //get rid of ' ' at beginning
  {
    i++;
  }
  myargv[index++]=&cmdline[i];  //add 1st nonempty char to myargv
  for(len=strlen(cmdline);i<len;i++)
  {
    if(cmdline[i]==' ')
      cmdline[i]=0;
    else
    {
      if((i-1>=0)&&(cmdline[i-1]==0))
      {
	myargv[index++]=&cmdline[i];
      }
    }
  }
  myargv[index]=NULL; //last position of myargv as null
  path="/bin/";
  //if(!strcmp(myargv[0],"grep")&&index==3)
 // {
  //  strcat(path,myargv[0]);
  //  strcat(path," ");
  //  strcat(path,myargv[2]);
  //  strcat(path," ");
  //  strcat(path,myargv[1]);
    
 // }
 // else{
  for(i=0;i<index;i++){
    //printf("myargv[%d] %s \n",i,myargv[i]);
    if(i>0){strcat(path," "); }
    strcat(path,myargv[i]);    
  }
 // }
  //strcat(path,myargv[0]);
  //if(myargv[1]){
  //  strcat(path," ");
  //  strcat(path,myargv[1]);
 // }
  //printf("\n goint to exec %s\n",path);
  exec(path);  //eq to exec(head);
  printf("exec failed\n");
  exit(1);
}

int scan(char *cmdline, char *head, char *tail)
{
  char cpy[128],*p,*h;
  int ch='|',i=0,j=0;
  strcpy(cpy,cmdline);
  //printf("scan ... cmdline is %s cpy is %s\n",cmdline,cpy);
  p=strrchr(cpy,ch);
  //printf("scan ... p is %s\n",p);
  h=&cpy;
  while(p)
  {
    
    while(h!=p)
    {
     // printf("*h is %c",*h);
      head[i]=*h;
      h++;
      i++;
    }
    head[i]='\0';
   // printf("scan head is %s",head);
    p++;
    while(*p)
    {
      tail[j]=*p;
      p++;
      j++;
    }
    tail[j]='\0';
    //printf("scan tail is %s\n",tail);
    if(!*tail)
    {
      return 0;
    }else
    {
      return 1;
    }

  }
  return 0;
}


main(int argc,char* argv[])
{
  char cmdline[128],temp[128],*token,cmd[64],content[64];
  int pid,status;
  
  while(1)
  {
    //get a command line; e.g. comline =  cmd | cmd2 ...
    printf("YHhaha.Sh:");gets(cmdline);
    strcpy(temp,cmdline);
    token=strtok(temp," ");

    if(token!=NULL)
    {
      strcpy(cmd,token);
      if(!strcmp(cmd,"cd"))
      {
	printf("cmd is cd\n");
	token=strtok(NULL," ");
	strcpy(cmd,token);
        printf("chdir to %s\n",cmd);
	chdir(cmd);
	continue;
      }
      if(!strcmp(cmd,"logout"))
      {
	printf("cmd is logout\n");
	exit(0);
	continue;
      }
      if(!strcmp(cmd,"pwd"))
      {
	printf("cmd is pwd\n");
	getcwd(content);
	printf("cwd is %s",content);
	continue;
      }
      pid=fork();    //fork a child sh process for binary executable command
      if(pid)
      {
	//parent
	printf("parent %d waiting for child to die...\n",getpid());
	pid=wait(&status);
	continue;
      }else
      {
	//child
        printf("child %d do_pipe %s\n",pid,cmdline);
        do_pipe(cmdline);
      }

    }
  }
}
