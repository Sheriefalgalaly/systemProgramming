#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
/*                                      YOU CAN SEE DOCUMENTATION IN REEADME FILE          */

#define BUF_SIZE 100
int main(int argc, char *argv[])
{
	// Creating Local var
	char *username;
	char cwd[100];
	char buf[BUF_SIZE];
	char *home = getenv("HOME");
	// Get the user name
	if ((username = getlogin()) == NULL)
		exit(0);

	while (1)
	{
		/*____________________________________________________*/
		// at each new prompt get the current working directory to display
		/*____________________________________________________*/
		getcwd(cwd, 100);

		printf("%s:%s$ ", username, cwd);

		int i = 0;

		/*____________________________________________________*/
		// Get the command from the user
		/*____________________________________________________*/
		fgets(buf, BUF_SIZE, stdin);
		buf[strlen(buf) - 1] = 0;
		if (strlen(buf) == 0)
			continue;
		/*____________________________________________________*/
		// Counting number of arguments
		/*____________________________________________________*/
		char ch_ = '#';
		int j = 0;
		int num_args = 0;

		char * buftemp = strdup(buf);
		char *tokentemp = strtok(buftemp, " '");
		while (buftemp != NULL)
		{
			buftemp = strtok(NULL, " '");
			num_args++;
		}
		
		// while (ch_ != 0)
		// {
		// 	ch_ = buf[j++];
		// 	// counting the spaces
		// 	if (ch_ == ' ')
		// 		num_args++;
		// }
		// num_args++;
		/*____________________________________________________*/
		// tokenize the input: to get "First command"
		/*____________________________________________________*/

		char *token = strtok(buf, " '");

		/*^^^^^^^^^^^^^^^^^^^^^    ECHO            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		if (!strcmp(token, "echo"))
		{
			while (token != NULL)
			{
				token = strtok(NULL, " "); // continue
				if (token != NULL)
				// printf("%s \n", token);
				{
					write(1, token, strlen(token));
					write(1," ",1);
				}
			}
			printf("\r");
		}
		/*^^^^^^^^^^^^^^^^^^^^^    EXIT            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		else if (!strcmp(token, "exit"))
		{
			printf("bye bye friend\n");
			exit(0);
		}
		/*^^^^^^^^^^^^^^^^^^^^^    PWD            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		else if (!strcmp(token, "pwd"))
		{
			getcwd(cwd, 100);
			printf("%s \n", cwd);
		}
		/*^^^^^^^^^^^^^^^^^^^^^    CD            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		else if (!strcmp(token, "cd"))
		{
			// $cd
			if(num_args == 1)
			{
				//home directory
				chdir(home);
				continue;
			}
			//$cd /
			else if (num_args == 2 && buf[3] == '/')
			{
				//root directory
				chdir("/");
				continue;
			}
			while (token != NULL)
			{
				token = strtok(NULL, " /"); // Get the next token

				if (token != 0 && strcmp(token, "~") == 0)
				{

					chdir(home);
				
				}
				else if (token != 0 && (chdir(token) != 0))
				{
					// Display the error and the token causes to the error
					printf("pico shell:cd: %s: %s\n", getcwd(cwd, 100), strerror(errno));
					break;
				}
			}
		}
		/*^^^^^^^^^^^^^^^^^^^^^    ANOTHER COMMAND           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		else
		{
			pid_t pid = fork();
			if (pid > 0)
			{
				int status;
				wait(&status);
				WEXITSTATUS(status);
			}
			else if (pid == 0)
			{

				char **newargv = malloc(sizeof(char *) * (num_args + 1));
				// Tokenize the buffer
				char *cmd = token;

				int i = 0; // Index for newargv
				while (token != NULL)
				{
					newargv[i++] = token;
					token = strtok(NULL, " '"); // Get the next token
				}
				newargv[i] = NULL;

				execvp(cmd, newargv);
				printf("Exec failed\n");
				exit(-1);
			}
			else
			{
				printf("failed to fork\n");
			}
		}
	}
}
