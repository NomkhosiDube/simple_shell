#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 256
#define MAX_NUM_ARGS 64


/**
 * is_exit_command - Check if the command is "exit".
 * @command: The command to check.
 *
 * Return: 1 if the command is "exit", 0 otherwise.
 *
 */


int is_exit_command(char *command)
{

	return (strcmp(command, "exit") == 0);
}


/**
 * execute_exit_command - Execute the "exit" command.
 * @args: The arguments passed to the command.
 */


int execute_exit_command(char *args[])

{


	if (args[1] != NULL)
	{
	int exit_status = atoi(args[1]);
		exit(exit_status);
	}
	else
	{
		exit(0);
	}
}


/**
 * execute_command - Execute a general command.
 * @args: The arguments passed to the command.
 */


int execute_exit_command(char *args[])
{


	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	
}

        else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
}


/**
 * main - Main function of the shell.
 *
 * Return: Always 0.
 */


int main(void)

{

	
	char input[MAX_INPUT_SIZE];

	while (1)
	{
		printf(":) ");
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			break;
		}
		input[strcspn(input, "\n")] = '\0';

		char *args[MAX_NUM_ARGS];
		int num_args = 0;

		char *token = strtok(input, " ");
		while (token != NULL && num_args < MAX_NUM_ARGS)
		{
			args[num_args++] = token;
			token = strtok(NULL, " ");
		}

		if (num_args > 0)
		{
			args[num_args] = NULL;

			if (is_exit_command(args[0]))
			{
				execute_exit_command(args);
			}
			else
			{
				execute_command(args);
			}
		}
	}

	return (0);
}



