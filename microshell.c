#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUF_SIZE 100
extern char **environ;

// ************** Redirection vars

int fd_in = 0xffff;
int fd_out = 0xffff;
int fd_err = 0xffff;

int fd_in_saved = 0xffff;
int fd_out_saved = 0xffff;
int fd_err_saved = 0xffff;
// ************* redirection prototypes
void open_input(char *file);
void open_output(char *file);
void open_err(char *file);

void restore_output(int saved_stdout);
void restore_err(int saved_stdout);
void restore_in(int saved_stdout);
// *************  main
int main(int argc, char *argv[])
{
	umask(0);
	// Creating Local var
	char *username;
	char cwd[100];
	char buf[BUF_SIZE];
	char *home = getenv("HOME");

	// local var
	typedef struct
	{
		char *key;
		char *val;
	} local_var_struct;

	local_var_struct *local_var_st[10];

	int local_count = 0;
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
		// Check redirection operation
		/*____________________________________________________*/
		char *out_char;
		char *in_char;
		char *err_char;

		err_char = strstr(buf, "2>");

		in_char = strchr(buf, '<');

		out_char = strstr(buf, " >");

		/* ---    Tokenize to get the file name and open files*/
		if (err_char != NULL)
		{
			char *file = strtok(err_char + 2, " ");
			open_err(file);
			err_char[0] = 0;
		}
		if (in_char != NULL)
		{
			char *file = strtok(in_char + 1, " ");
			open_input(file);
			in_char[0] = 0;
		}
		if (out_char != NULL)
		{
			char *file = strtok(out_char + 2, " ");
			open_output(file);
			out_char[0] = 0;
		}

		/*____________________________________________________*/
		// Counting number of arguments
		/*____________________________________________________*/
		char ch_ = '#';
		int j = 0;
		int num_args = 0;

		char *buftemp = strdup(buf);
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
		char *firsteq = strrchr(token, '=');
		/*^^^^^^^^^^^^^^^^^^^^^    ECHO            ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		if (!strcmp(token, "echo"))
		{

			while (token != NULL)
			{
				char *dollar = NULL;
				char *val = NULL;
				token = strtok(NULL, " "); // continue

				if (token != NULL )
				{
					dollar = strrchr(token, '$');
					if(dollar!= NULL) val = getenv(dollar + 1);
				}
				// search the temp key in environemnt var
				if (val != NULL)
				{
					write(1, val, strlen(val));
					write(1, " ", 1);
					continue;
				}

				if (dollar != NULL)
				{
					int iterator = 0;
					while (iterator < local_count)
					{
						if (!strcmp(local_var_st[iterator]->key, &dollar[1]))
						{
							write(1, local_var_st[iterator]->val, strlen(local_var_st[iterator]->val));
							write(1, " ", 1);
						}
						iterator++;
					}
				}
				else if (token != NULL)
				// printf("%s \n", token);
				{
					write(1, token, strlen(token));
					write(1, " ", 1);
				}
			}
			printf("\n\r");
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
			if (num_args == 1)
			{
				// home directory
				chdir(home);
				continue;
			}
			//$cd /
			else if (num_args == 2 && buf[3] == '/')
			{
				// root directory
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
		else if (num_args == 1 && firsteq != NULL)
		{
			// make sure arg has "="
			int keyFound = 0;
			// the next char
			firsteq++;

			// Get length
			int keylength = firsteq - token - 1;
			int valLenght = strlen(token) - keylength - 1;

			// search first if this variable exists or no
			char tempkey[keylength + 1];
			strncpy(tempkey, token, keylength);
			tempkey[keylength] = 0;

			int iterator = 0;
			while (iterator < local_count)
			{
				if (local_var_st[iterator] != NULL && !strcmp(local_var_st[iterator]->key, tempkey))
				{
					free(local_var_st[iterator]->val);
					local_var_st[iterator]->val = malloc((valLenght + 1) * sizeof(char));
					strncpy(local_var_st[iterator]->val, firsteq, valLenght);
					keyFound = 1;
				}
				iterator++;
			}
			if (!keyFound)
			{
				// find first index in array = NULL
				int iterator = 0;
				while (iterator < 10)
				{
					if (local_var_st[iterator] == NULL)
						break;
					iterator++;
				}

				// equal is found
				local_var_st[iterator] = malloc(sizeof(local_var_st));

				local_var_st[iterator]->key = malloc((keylength + 1) * sizeof(char));
				local_var_st[iterator]->val = malloc((valLenght + 1) * sizeof(char));

				// extract the value
				strncpy(local_var_st[iterator]->key, token, keylength);
				strncpy(local_var_st[iterator]->val, firsteq, valLenght);
				local_var_st[iterator]->key[keylength] = 0;
				local_var_st[iterator]->val[valLenght] = 0;

				local_count++;
			}
		}
		/*^^^^^^^^^^^^^^^^^^^^^    Printing local vars           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
		else if (!strcmp(token, "set"))
		{
			int iterator = 0;

			while (iterator < local_count)
			{
				if (local_var_st[iterator] != NULL && local_var_st[iterator]->key != NULL)
					printf("%s=%s\n", local_var_st[iterator]->key, local_var_st[iterator]->val);
				iterator++;
			}
		}
		else if (!strcmp(token, "unset"))
		{

			while (token != NULL)
			{
				int iterator = 0;
				token = strtok(NULL, " "); // Get the next token

				// search in env var
				if (token != NULL && getenv(token) != NULL)
				{
					unsetenv(token);
				}

				else if (token != NULL)
				{
					// search in local vars
					while (iterator < local_count)
					{
						if (!strcmp(local_var_st[iterator]->key, token))
						{
							free(local_var_st[iterator]->val);
							free(local_var_st[iterator]->key);
							free(local_var_st[iterator]);

							local_var_st[iterator]->key = NULL;
							local_var_st[iterator]->val = NULL;
							local_var_st[iterator] = NULL;
						}
						iterator++;
					}
				}
			}
		}
		else if (!strcmp(token, "export"))
		{

			token = strtok(NULL, " "); // Get the next token
			char *tokentemp = strdup(token);
			if (putenv(tokentemp) == 0)
				printf("success\n");
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
					// newargv[i++] = token;
					// newargv[i] = malloc(strlen(token) + 1);
					// strcpy(newargv[i++],token);
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

		// check standard file
		if (fd_out_saved != 0xffff)
		{
			// it must restore the stdout to 1
			restore_output(fd_out_saved);
		}
		// check standard file
		if (fd_err_saved != 0xffff)
		{
			// it must restore the stdout to 1
			restore_err(fd_err_saved);
		}
		// check standard file
		if (fd_in_saved != 0xffff)
		{
			// it must restore the stdout to 1
			restore_in(fd_in_saved);
		}
	}
}

void open_input(char *file)
{
	// Save the current stdin
	fd_in_saved = dup(STDIN_FILENO);
	if (fd_in_saved == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	// //open input file

	if ((fd_in = open(file, O_RDONLY)) < 0)
	{
		printf("he shell could not open the file for reading in \n");
		exit(-1);
	}

	if (dup2(fd_in, 0) == -1)
	{
		printf("error in opening input file\n");
	}
	close(fd_in);
}
void restore_in(int saved_stdout)
{
	// Restore the original stdout
	if (dup2(saved_stdout, STDIN_FILENO) == -1)
	{
		perror("dup2 (restore)");
		exit(EXIT_FAILURE);
	}
	fd_in_saved = 0xffff;
	close(saved_stdout); // No longer needed
}
void open_output(char *file)
{
	// Save the current stdout
	fd_out_saved = dup(STDOUT_FILENO);
	if (fd_out_saved == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	// //open output file
	if ((fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
	{
		printf("he shell could not open the file for writing\n");
		exit(-1);
	}
	// printf("fff\n");
	if (dup2(fd_out, 1) == -1)
	{
		printf("error in opening output file\n");
	};

	close(fd_out);
}
void restore_output(int saved_stdout)
{
	// Restore the original stdout
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 (restore)");
		exit(EXIT_FAILURE);
	}
	fd_out_saved = 0xffff;
	close(saved_stdout); // No longer needed
}
void open_err(char *file)
{
	// Save the current stdout
	fd_err_saved = dup(STDERR_FILENO);
	// //open erro file

	if ((fd_err = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
	{
		printf("the shell could not open the file for writing\n");
		exit(-1);
	}
	// printf("fff\n");
	if (dup2(fd_err, 2) == -1)
		printf("error in opening error file\n");
	close(fd_err);
}
void restore_err(int saved_stdout)
{
	// Restore the original stdout
	if (dup2(saved_stdout, STDERR_FILENO) == -1)
	{
		perror("dup2 (restore)");
		exit(EXIT_FAILURE);
	}
	fd_err_saved = 0xffff;
	close(saved_stdout); // No longer needed
}
