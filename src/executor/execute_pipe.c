/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                          PIPE EXECUTION HELPERS                           */
/* ========================================================================== */

/**
 * execute_left_pipe - Esegue il comando a sinistra della pipe
 * @node: Nodo AST da eseguire
 * @pipe_fd: File descriptors della pipe
 * @ctx: Contesto della shell
 * 
 * Reindirizza stdout sulla write end della pipe ed esegue il comando left.
 */
static void	execute_left_pipe(t_ast_node *node, int pipe_fd[2],
				t_shell_context *ctx)
{
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[1]);
	exit(execute(node->left, ctx));
}

/**
 * execute_right_pipe - Esegue il comando a destra della pipe
 * @node: Nodo AST da eseguire
 * @pipe_fd: File descriptors della pipe
 * @ctx: Contesto della shell
 * 
 * Reindirizza stdin sulla read end della pipe ed esegue il comando right.
 */
static void	execute_right_pipe(t_ast_node *node, int pipe_fd[2],
				t_shell_context *ctx)
{
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[0]);
	exit(execute(node->right, ctx));
}

/* ========================================================================== */
/*                         MAIN PIPE EXECUTION                               */
/* ========================================================================== */

/**
 * execute_pipe - Esegue una pipeline (comando1 | comando2)
 * @node: Nodo AST di tipo NODE_PIPE
 * @ctx: Contesto della shell
 * 
 * Algoritmo:
 * 1. Crea una pipe
 * 2. Fork primo figlio per left:
 *    - stdout → write end della pipe
 *    - esegue left
 * 3. Fork secondo figlio per right:
 *    - stdin ← read end della pipe
 *    - esegue right
 * 4. Padre chiude entrambi i file descriptors della pipe
 * 5. Padre aspetta entrambi i figli
 * 6. Return exit status dell'ultimo comando (right)
 * 
 * Return: Exit status del comando right
 */
int	execute_pipe(t_ast_node *node, t_shell_context *ctx)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;
	int		exit_status;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	pid_left = fork();
	if (pid_left < 0)
		return (perror("fork"), 1);
	if (pid_left == 0)
		execute_left_pipe(node, pipe_fd, ctx);
	pid_right = fork();
	if (pid_right < 0)
		return (perror("fork"), 1);
	if (pid_right == 0)
		execute_right_pipe(node, pipe_fd, ctx);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = 1;
	return (exit_status);
}


