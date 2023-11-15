#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute_command: Calls function with the args array.
 * Main: Commands with the arguments "ls" and "-l" in the args array.
 *
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

int main(void) {

char *args[] = {"ls", "-l", NULL};

execute_command(args);

    return 0;
}
