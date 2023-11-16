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
    char *token = strtok(path, ":");

    while (token != NULL) {
        char *executable = malloc(strlen(token) + strlen(command) + 2);
        sprintf(executable, "%s/%s", token, command);

        if (access(executable, X_OK) == 0) {
            return executable;
        }

        free(executable);
        token = strtok(NULL, ":");
    }

    return NULL;
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
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
        char *token;
        char *args[MAX_ARGS];
        int arg_count = 0;

        token = strtok(input, " ");
        while (token != NULL && arg_count < MAX_ARGS) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }

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

    return 0;
}

