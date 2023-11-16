#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

void display_prompt() {
    printf("#hsh$ ");
    fflush(stdout);
}

char *find_executable(const char *command) {
    char *path = getenv("PATH");
    char *dir;
    char *token = strchr(path, ':');

    while (token != NULL) {
        char *executable = malloc(token - path + strlen(command) + 2);
        strncpy(executable, path, token - path);
        executable[token - path] = '\0';
        strcat(executable, "/");
        strcat(executable, command);

        if (access(executable, X_OK) == 0) {
            return executable;
        }

        free(executable);
        path = token + 1;
        token = strchr(path, ':');
    }

    return NULL;
}

void tokenize_input(const char *input, char **args, int *arg_count) {
    const char *delimiters = " \t\n";
    char *copy = strdup(input);
    char *token = strtok(copy, delimiters);

    while (token != NULL && *arg_count < MAX_ARGS) {
        args[(*arg_count)++] = token;
        token = strtok(NULL, delimiters);
    }

    free(copy);
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int arg_count;

    while (1) {
        arg_count = 0;
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            // User entered "exit," terminate the shell
            break;
        }

        // Tokenize the input into command and arguments
        tokenize_input(input, args, &arg_count);

        if (arg_count > 0) {
            args[arg_count] = NULL;

            // Find the executable in the PATH
            char *executable = find_executable(args[0]);

            if (executable == NULL) {
                printf("%s: command not found\n", args[0]);
                continue;
            }

            pid_t pid = fork();

            if (pid == -1) {
                perror("fork");
                free(executable);
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                execvp(executable, args);
                perror(executable);
                free(executable);
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                int status;
                waitpid(pid, &status, 0);

                free(executable);
            }
        }
    }

    return 0;
}

