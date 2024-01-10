#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *custom_getline() 
{
    static char buffer[BUFFER_SIZE];
    static size_t position = 0;
    static ssize_t bytes_read = 0;

    if (bytes_read <= 0) 
    {
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) 
	{
            return NULL;
        }
        position = 0;
    }

    size_t start = position;
    size_t end = position;

    while (end < position + bytes_read && buffer[end] != '\n') 
    {
        end++;
    }

    size_t length = end - start;
    char *line = malloc(length + 1);
    if (line == NULL) 
    {
        return NULL;
    }

    memcpy(line, buffer + start, length);
    line[length] = '\0';

    if (end < position + bytes_read) 
    {
        position = end + 1;
        bytes_read -= length + 1;
    } 
    else 
    {
        position = 0;
        bytes_read = 0;
    }

    return line;
}

int main() 
{
    char *line;
    while ((line = custom_getline()) != NULL) 
    {
        printf("Read line: %s\n", line);
        free(line);
    }
    return (0);
}

