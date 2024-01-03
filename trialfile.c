#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
* main - PID
* @argc: number of arguments provided
* @argv: an array of all arguments
*
* Return: an int
*/

int main(int argc, char *argv[]) 
{
if (argc > 1) 
{
/* Non-interactive mode */
char **args = &argv[1]; /* Use the command-line argument as the command */
execve(args[0], args, NULL); /* Execute the specified command */
perror("execve"); /* This line will only be reached if execve fails */
return (1);
}

/* Interactive mode */
char *command = NULL;
size_t bufsize = 0;

while (1) 
{
printf("#cisfun$ "); /* Display the prompt */
ssize_t read = getline(&command, &bufsize, stdin);

if (read == -1) 
{
if (feof(stdin)) 
{
printf("\n");
break; /* End of file condition (Ctrl+D) */
} 
else
{
perror("getline");
free(command); /* Free the dynamically allocated buffer */
return (1);
}
}

/* Remove the newline character at the end of the command */
if (command[read - 1] == '\n') 
{
command[read - 1] = '\0';
}

char *args[2];
args[0] = command;
args[1] = NULL;

pid_t pid = fork(); /* Create a child process */
if (pid == -1) 
{
perror("fork");
free(command); /* Free the dynamically allocated buffer */
return (1);
} 
else if (pid == 0) 
{
/* Child process */
execve(args[0], args, NULL); /* Execute the command */
perror("execve"); /* This line will only be reached if execve fails */
free(command); /* Free the dynamically allocated buffer */
return (1);
} 
else 
{
/* Parent process */
int status;
if (waitpid(pid, &status, 0) == -1) 
{
perror("waitpid");
free(command); /* Free the dynamically allocated buffer */
return (1);
}
}
}

free(command); /* Free the dynamically allocated buffer */
return (0);
}

