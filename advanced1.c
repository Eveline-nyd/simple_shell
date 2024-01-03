#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char *custom_getline() {
    static char buffer[BUFFER_SIZE];
    static size_t position = 0;
    static ssize_t bytes_read = 0;

    if (bytes_read <= 0) {
        // If there are no bytes left in the buffer, read more input
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            return NULL; // End of file or error
        }
        position = 0; // Reset position to the beginning of the buffer
    }

    // Find the end of the line or the end of the buffer
    size_t i;
    for (i = position; i < position + bytes_read; i++) {
        if (buffer[i] == '\n' || buffer[i] == '\0') {
            break; // Found the end of the line or end of buffer
        }
    }

    // Allocate memory for the line and copy it from the buffer
    char *line = malloc(i - position + 1);
    if (line == NULL) {
        return NULL; // Memory allocation error
    }
    memcpy(line, buffer + position, i - position);
    line[i - position] = '\0'; // Null-terminate the line

    // Update the buffer position and remaining bytes
    position = i + 1;
    bytes_read -= (i - position);

    return line;
}

int main() {
    char *line;
    while ((line = custom_getline()) != NULL) {
        printf("Read line: %s\n", line);
        free(line);
    }
    return 0;
}

