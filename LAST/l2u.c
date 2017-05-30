/*******************l2u.c**********************/
#include "ucode.c"

main(argc, char* argv[])
{
  int n;
  char c;
  write(2,"###### Y.H L2U ######\n\r",23);
  while(read(0,&c,1))
  {
    if(c>=97 && c<=122)
    {
      c-=32;
    }
    write(1,&c,1);
    if(c=='\n')
      write(2,"\r",1);
    else if(c=='\r')
    {
      write(1,"\n",1);
      write(2,"\r",1);
    }
  }
  exit(0);
}
