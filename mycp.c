#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h> // For umask()
char buf[100];

int main(int argc, char *argv[])
{
    umask(0); // ?!!!!!!!!!
     
    char *destName;
    char *token;
    char *srcName;

    char* srcPath = strdup(argv[1]);

    /************* Opening the src file ***********/ 
    int fd_src;
    fd_src = open(argv[1], O_RDONLY);

    // check if the src is a file
    if (errno == EISDIR)
    {
        perror("src isn't a file\n");
        exit(0);
    }
    else if (fd_src < 0)
    {
        perror("Error opening src file\n");
        exit(0);
    }
    /************ getting the name of the file of th src ******* */

    // iterating until get the final name of the file
    token = strtok(argv[1], "/");
    while (token != NULL)
    {
        // printf("Token: %s\n", token);
        srcName = token;
        token = strtok(NULL, "/");
    }
/*---------------------------------------------------------------------------------------------------*/
    /************* opening the destination file  **************/
    int fd_dest;
    fd_dest = open(argv[2], O_CREAT | O_WRONLY, 0777);

    // check if the destination is a directory
    if (errno == EISDIR)
    {
        // concat src path name with the directory
        int length = strlen(argv[2]);
        char dest[length * 2]; 
        sprintf(dest, "%s/%s", argv[2], srcName);
        if(strcmp(dest,srcPath))
        {
           printf("'%s' and '%s'two files have the same name \n",dest,srcPath);
           exit(0);
        }
        //reopen the file
        fd_dest = open(dest, O_CREAT | O_WRONLY, 0777);
    }

    if (fd_dest < 0)
    {
        perror("error in opening the dest file\n");
        exit(0);
    }

    // writing the file data to destination
    int count;
    while ((count = read(fd_src, buf, 100)) > 0)
    {
        if (write(fd_dest, buf, count) < 0)
            perror("error in writing in the destination\n");
    }
    close(fd_src);
    close(fd_dest);
}
