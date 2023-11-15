#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute_command:prints functions to execute the command
 * Description: The main function implements a 
 * simple shell loop that repeatedly prompts
 * the user with a "$" symbol.
 *
 * Return: 0
*/

void execute_command(char *args[void]) {
    pid_t pid = fork();

// Check for fork failure
    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {

 // Child process      
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
 // Parent process      
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

// Main shell loop
    while (1) {
        printf("$ ");

// Read user input	
        fgets(input, sizeof(input), stdin);

 // Remove trailing newline character
        input[strcspn(input, "\n")] = '\0';

     
        char *args[MAX_NUM_ARGS];
        int num_args = 0;

// Tokenize input into arguments
        char *token = strtok(input, " ");
        while (token != NULL && num_args < MAX_NUM_ARGS) {
            args[num_args++] = token;
            token = strtok(NULL, " ");
        }
// Check if there are any arguments
        if (num_args > 0) {
            args[num_args] = NULL;

 // Execute the command	    
            execute_command(args);
        }
    }

    return 0;
}
