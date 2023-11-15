#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute_command: Takes an array of command-line arguments.
 * Main Function: Checks if the first argument is the exit command
 *
 * Returns: 0
 */

void execute_command(char *args[]) {
    char *path = getenv("PATH");
    char *token = strtok(path, ":");

    while (token != NULL) {
        char executable_path[MAX_INPUT_SIZE];
        snprintf(executable_path, sizeof(executable_path), "%s/%s", token, args[0]);

        if (access(executable_path, X_OK) == 0) {
            pid_t pid = fork();

            if (pid == -1) {
                perror("fork");
            } else if (pid == 0) {
                if (execvp(executable_path, args) == -1) {
                    perror("execvp");
                    exit(EXIT_FAILURE);
                }
            } else {
                int status;
                waitpid(pid, &status, 0);
                return;
            }
        }

        token = strtok(NULL, ":");
    }

    fprintf(stderr, "%s: command not found\n", args[0]);
}

int is_exit_command(char *command) {
    return strcmp(command, "exit") == 0;
}

int is_env_command(char *command) {
    return strcmp(command, "env") == 0;
}

void print_environment() {
    extern char **environ;
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf(":) ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        char *args[MAX_NUM_ARGS];
        int num_args = 0;

        char *token = strtok(input, " ");
        while (token != NULL && num_args < MAX_NUM_ARGS) {
            args[num_args++] = token;
            token = strtok(NULL, " ");
        }

        if (num_args > 0) {
            args[num_args] = NULL;

            if (is_exit_command(args[0])) {
                break;
            } else if (is_env_command(args[0])) {
                print_environment();
            } else {
                execute_command(args);
            }
        }
    }

    return 0;
}

