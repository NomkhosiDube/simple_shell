#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* _getline: function to read a line of text from stdin
 * main:function contains an infinite loop that 
 * *continuously prompts the user for input.
 * Return: 0
 */ 

char *_getline() {
    static char buffer[BUFFER_SIZE];
    static size_t buffer_index = 0;
    static size_t buffer_size = 0;

    char *line = NULL;
    size_t line_size = 0;
    int c;

    while (1) {
        if (buffer_index >= buffer_size) {
            buffer_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (buffer_size <= 0) {
                
                break;
            }
            buffer_index = 0;
        }

        c = buffer[buffer_index++];
        if (c == '\n' || c == EOF) {
            
            break;
        }

        line = realloc(line, line_size + 1);
        if (!line) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        line[line_size++] = (char)c;
    }

    if (line_size > 0) {
        line = realloc(line, line_size + 1);
        if (!line) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        line[line_size] = '\0';
    }

    return line;
}

int main() {
    while (1) {
        char *line = custom_getline();
        if (!line) {
            break; 
        }

        printf("You entered: %s\n", line);

        free(line);
    }

    return 0;
}
