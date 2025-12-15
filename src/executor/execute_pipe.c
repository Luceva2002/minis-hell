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

static int	handle_pipe_signal(int status)
{
	if (WTERMSIG(status) == SIGINT)
		return (write(1, "\n", 1), 130);
	if (WTERMSIG(status) == SIGQUIT)
		return (ft_putendl_fd("Quit: 3", 2), 131);
	return (128 + WTERMSIG(status));
}

static int	wait_pipe_children(pid_t left, pid_t right)
{
	int	status;

	waitpid(left, &status, 0);
	waitpid(right, &status, 0);
	if (WIFSIGNALED(status))
		return (handle_pipe_signal(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_pipe(t_ast_node *node, t_shell_context *ctx)
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;

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
	return (wait_pipe_children(pid_left, pid_right));
}
