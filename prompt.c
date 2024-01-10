#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - starting point
 * prompt-function - displays a command prompt
 *
 * Return: 0 when success
 */
int main(void)
{
	char *command = NULL;
		size_t bufsize = 0;

		while (1)
		{
			printf("simple_shell>"); /*Display prompt */
			ssize_t read = getline(&command, &bufsize, stdin);

			if (read == -1)
			{
				if (feof(stdin)) 
				{
					printf("\n");
					break;/*End of file conditon ctrl+D*/
				} 
				else
				{
					perror("getline");
					exit(EXIT_FAILURE);
				}
			}
			/*REMOVE NEWLINE CHARACTER AT THE END OF THE COMMAND */
			if (command[read - 1] == '\n')
			{
				command[read - 1] = '\0';
			}
			char *args[2];

			args[0] = command;
			args[2] = NULL;
			pid_t pid = fork();/*create a child process*/
			if (pid == -1);
			{
			perror("fork");
			exit(EXIT_FAILURE);
			}
			else if (pid == 0) 
			{
				/*child process*/
				execve(args[0], args, NULL); /*execute the command */
				perror("execve"); /*will only be reached if execve fails*/
				exit(EXIT_FAILURE);
			}
			else 
			{
				/*parent process*/
				int status;

				if (waitpid(pid, &status, 0) == -1)
				{
					perror("waitpid");
					exit(EXIT_FAILURE);
				}
			}

			if (WIFEXITED(status)) 
			{
				/*child process exited normally*/
				if (WEXISTATUS(status) == 127)
				{
					printf("child process did not exit normally\n");
				}
			}
		}
		free(command); /*free the dynamically allocated memory*/
		return (0);
}

					
