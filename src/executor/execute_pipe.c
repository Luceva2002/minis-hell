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
#include <signal.h>

static void	execute_left_pipe(t_ast_node *node, int pipe_fd[2],
				t_shell_context *ctx)
{
	setup_signals_child();
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[1]);
	exit(execute(node->left, ctx));
}

static void	execute_right_pipe(t_ast_node *node, int pipe_fd[2],
				t_shell_context *ctx)
{
	setup_signals_child();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[0]);
	exit(execute(node->right, ctx));
}

static int	wait_pipe_children(pid_t left, pid_t right)
{
	int	status;

	waitpid(left, &status, 0);
	waitpid(right, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (write(1, "\n", 1), 130);
		if (WTERMSIG(status) == SIGQUIT)
			return (ft_putendl_fd("Quit: 3", 2), 131);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	create_children(t_ast_node *node, t_shell_context *ctx,
		int pipe_fd[2], pid_t *pids)
{
	pids[0] = fork();
	if (pids[0] < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (perror("fork"), 1);
	}
	if (pids[0] == 0)
		execute_left_pipe(node, pipe_fd, ctx);
	pids[1] = fork();
	if (pids[1] < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pids[0], NULL, 0);
		return (perror("fork"), 1);
	}
	if (pids[1] == 0)
		execute_right_pipe(node, pipe_fd, ctx);
	return (0);
}

int	execute_pipe(t_ast_node *node, t_shell_context *ctx)
{
	int		pipe_fd[2];
	pid_t	pids[2];

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	if (create_children(node, ctx, pipe_fd, pids))
		return (1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_pipe_children(pids[0], pids[1]));
}
