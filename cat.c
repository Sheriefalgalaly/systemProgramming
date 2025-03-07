#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main (int argc, char * argv[])
{
    char buf[100];

    if(argc != 2)
    {
        perror("must provide a path to cat");
        exit(0);
    }
    int fd = open(argv[1],O_RDONLY);
    if(fd < 0)
    {
        printf("Error opening file: %s\n", strerror(errno));
         
    } 
    int count ;
    while((count = read(fd,buf,100)) > 0)
    {
     if(write(1,buf,count) < 0)
	 {
         perror("error is found");
       
        }
    } 	 
    close(fd);
    return 0;
}
