#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{

  char buf[100];
  if(argc != 1)
  {
	  exit(0);
  }
  if(strcmp(argv[0],"pwd"))
   {
    if( getcwd(buf, 100) == NULL)
	   exit(0);
     printf("%s\n",buf);    
   }
   return 0;
}
