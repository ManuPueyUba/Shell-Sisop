#include "exec.h"
#include <stdlib.h>
#include "defs.h"
#include "printstatus.h"

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	// Your code here
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	// Your code here
	int fd;
	fd = open(file, flags, S_IWUSR | S_IRUSR);

	if (fd == -1) {
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}
	return fd;
}

int
valid(char *file)
{
	return strlen(file) <= 0;
}


void
redirect_stdin(char *file)
{
	if (!valid(file)) {
		int fd = open_redir_fd(file, O_RDONLY);
		if (fd != STDIN_FILENO) {
			if (dup2(fd, STDIN_FILENO) != STDIN_FILENO) {
				perror("Error duplicating file descriptor");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
	}
}

void
redirect_stdout(char *file)
{
	if (!valid(file)) {
		int fd = open_redir_fd(file, O_WRONLY | O_CREAT | O_TRUNC);
		if (fd != STDOUT_FILENO) {
			if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
				perror("Error duplicating file descriptor");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
	}
}

void
redirect_stderr(char *file)
{
	if (!valid(file)) {
		int fd = open_redir_fd(file, O_WRONLY | O_CREAT | O_TRUNC);
		if (fd != STDERR_FILENO) {
			if (dup2(fd, STDERR_FILENO) != STDERR_FILENO) {
				perror("Error duplicating file descriptor");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
	}
}


// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC:
		// spawns a command
		// Your code here
		e = (struct execcmd *) cmd;
		set_environ_vars(e->eargv, e->eargc);
		if (e->argv[COMMAND_NAME] == NULL) {
			// No se encontro el comando
			printf("Command not found\n");
			exit(EXIT_FAILURE);
		}
		execvp(e->argv[COMMAND_NAME], e->argv);
		// Si llega aca es porque hubo un error
		perror("execvp failed");
		exit(EXIT_FAILURE);
		break;

	case BACK: {
		// runs a command in background
		//
		// Your code here
		printf("Background process are not yet implemented\n");
		_exit(-1);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here
		e = (struct execcmd *) cmd;
		redirect_stdin(e->in_file);
		redirect_stdout(e->out_file);
		redirect_stderr(e->err_file);

		printf("Redirections are not yet implemented\n");
		_exit(-1);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		printf("Pipes are not yet implemented\n");

		// free the memory allocated
		// for the pipe tree structure
		free_command(parsed_pipe);

		break;
	}
	}
}
