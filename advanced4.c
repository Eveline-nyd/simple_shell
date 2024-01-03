#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024

char *custom_getline() {
    // ... (same as previous implementation)
}

int main(int argc, char *argv[]) {
    char *command = NULL;
    size_t bufsize = 0;

    while (1) {
        printf("#cisfun$ "); /* Display the prompt */
        ssize_t read = getline(&command, &bufsize, stdin);

        if (read == -1) {
            if (feof(stdin)) {
                printf("\n");
                break; /* End of file condition (Ctrl+D) */
            } else {
                perror("getline");
                free(command); /* Free the dynamically allocated buffer */
                return 1;
            }
        }

        /* Remove the newline character at the end of the command */
        if (command[read - 1] == '\n') {
            command[read - 1] = '\0';
        }

        if (strcmp(command, "exit") == 0) {
            free(command); /* Free the dynamically allocated buffer */
            return 0; /* Exit the shell with status 0 */
        } else if (strncmp(command, "exit ", 5) == 0) {
            // ... (same as previous implementation)
        } else if (strncmp(command, "setenv ", 7) == 0) {
            /* Parse the setenv command */
            char *var_value = command + 7;
            char *var = strtok(var_value, " ");
            char *value = strtok(NULL, " ");

            if (var != NULL && value != NULL) {
                if (setenv(var, value, 1) != 0) {
                    fprintf(stderr, "Failed to set environment variable: %s\n", strerror(errno));
                }
            } else {
                fprintf(stderr, "Invalid setenv command syntax: %s\n", command);
            }
        } else if (strncmp(command, "unsetenv ", 9) == 0) {
            /* Parse the unsetenv command */
            char *var = command + 9;

            if (var != NULL) {
                if (unsetenv(var) != 0) {
                    fprintf(stderr, "Failed to unset environment variable: %s\n", strerror(errno));
                }
            } else {
                fprintf(stderr, "Invalid unsetenv command syntax: %s\n", command);
            }
        } else if (strcmp(command, "env") == 0) {
            // ... (same as previous implementation)
        } else {
            // ... (same as previous implementation)
        }
    }

    free(command); /* Free the dynamically allocated buffer */
    return 0;
}

