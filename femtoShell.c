#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// ****************            YOU CAN SEE Description in README file 
int main(int argc, char *argv[])
{
	char *username;
	if ((username = getlogin()) == NULL)
		exit(0);
	//	printf("%s",username);
	char cmd[50] = {'\n'};
	while (1)
	{
		printf("%s >", username);
		int i = 0;
		unsigned char ch = '#';

		while (ch != '\n')
		{
			ch = (unsigned char)getchar();
			cmd[i++] = ch;
		}
		cmd[i] = '\0';


		// tokenize the input
		char *token = strtok(cmd, " ");

		if (!strcmp(token, "echo"))
		{
			
			while (token != NULL)
			{
				token = strtok(NULL, " ");  // continue
				if(token != NULL )printf("%s ", token);
			}
			printf("\n");
		}
		else if (!strcmp(token, "exit"))
		{
			printf("bye bye friend\n");
			exit(0);
		}
		else
		{
			printf("invalid input\n");
			exit(0);
		}
	}
}
