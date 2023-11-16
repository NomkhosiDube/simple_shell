#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

char *custom_getline() {
    static char buffer[MAX_INPUT_SIZE];
    static size_t buffer_index = 0;
    static size_t buffer_size = 0;

    char *line = NULL;
    size_t line_size = 0;
    int c;

    while (1) {
        if (buffer_index >= buffer_size) {
            buffer_size = read(STDIN_FILENO, buffer, sizeof(buffer));
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

void display_prompt() {
    printf("#hsh$ ");
    fflush(stdout);
}

int main() {
    char *input;

    while (1) {
        display_prompt();

        input = custom_getline();

        if (input == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        printf("You entered: %s\n", input);

        free(input);
    }

    return 0;
}

