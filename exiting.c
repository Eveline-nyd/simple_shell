#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * main - entry point
 * exiting-function - displays a command exit
 * @argc:number of arguments provided
 * @argv: array of the arguments
 *
 * Return: 0 when success
 */

int main(int argc, char *argv[]) 
{
    if (argc > 1) 
    {
        /* Non-interactive mode */
        execve(argv[1], &argv[1], NULL); /* Execute the specified command with arguments */
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

        if (strcmp(command, "exit") == 0) 
	{
            break; /* Exit the loop and terminate the shell */
        }

        char *args[MAX_COMMAND_LENGTH];
        int arg_count = 0;
        char *token = strtok(command, " ");

        while (token != NULL && arg_count < MAX_COMMAND_LENGTH - 1) 
	{
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; /* Set the last element to NULL as required by execve */

        /* Check if the command exists in the PATH */
        char *path = getenv("PATH");
        char *path_copy = strdup(path);
        char *dir = strtok(path_copy, ":");
        int command_found = 0;

        while (dir != NULL) 
	{
            char command_path[MAX_COMMAND_LENGTH];
            snprintf(command_path, sizeof(command_path), "%s/%s", dir, args[0]);
            if (access(command_path, X_OK) == 0) 
	    {
                command_found = 1;
                execve(command_path, args, NULL);
                perror("execve"); /* This line will only be reached if execve fails */
                free(command); /* Free the dynamically allocated buffer */
                free(path_copy); /* Free the dynamically allocated buffer */
                return (1);
            }
            dir = strtok(NULL, ":");
        }

        if (!command_found) 
	{
            printf("%s: command not found\n", args[0]);
        }

        free(path_copy); /* Free the dynamically allocated buffer */
    }

    free(command); /* Free the dynamically allocated buffer */
    return (0);
}

