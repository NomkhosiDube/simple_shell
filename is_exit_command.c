#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* is_exit_command: Checks if the exit command is entered
 * Return: 0
 */

void execute_command(char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
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

void parse_input(char *input, char *args[]) {
    int num_args = 0;
    int arg_index = 0;
    char current_arg[MAX_ARG_SIZE];

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            if (arg_index > 0) {
                current_arg[arg_index] = '\0';
                args[num_args] = strdup(current_arg);
                num_args++;
                arg_index = 0;
            }
        } else {
            current_arg[arg_index] = input[i];
            arg_index++;
        }

        if (num_args >= MAX_NUM_ARGS - 1) {
            break;
        }
    }

    if (arg_index > 0) {
        current_arg[arg_index] = '\0';
        args[num_args] = strdup(current_arg);
        num_args++;
    }

    args[num_args] = NULL;
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
        parse_input(input, args);

        if (args[0] != NULL) {
            if (is_exit_command(args[0])) {
                if (args[1] != NULL) {
                    int exit_status = atoi(args[1]);
                    exit(exit_status);
                } else {
                    break;
                }
            } else if (is_env_command(args[0])) {
                print_environment();
            } else {
                execute_command(args);
            }
        }

        for (int i = 0; args[i] != NULL; i++) {
            free(args[i]);
        }
    }

    return 0;
}

