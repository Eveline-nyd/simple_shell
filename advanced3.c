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
            /* Extract the status argument */
            char *status_str = command + 5;
            char *endptr;
            errno = 0;
            long status = strtol(status_str, &endptr, 10);
            if (errno != 0 || *endptr != '\0') {
                printf("Invalid status argument: %s\n", status_str);
            } else {
                free(command); /* Free the dynamically allocated buffer */
                return (int)status; /* Exit the shell with the specified status */
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

