/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

static int	handle_signal_status(int status)
{
	if (WTERMSIG(status) == SIGINT)
		return (write(1, "\n", 1), 130);
	if (WTERMSIG(status) == SIGQUIT)
		return (ft_putendl_fd("Quit: 3", 2), 131);
	return (128 + WTERMSIG(status));
}

static int	execute_subshell(t_ast_node *node, t_shell_context *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		setup_signals_child();
		exit(execute(node->left, ctx));
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (handle_signal_status(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	execute_seq(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	ctx->last_exit_code = exit_status;
	exit_status = execute(node->right, ctx);
	return (exit_status);
}

int	execute(t_ast_node *ast, t_shell_context *ctx)
{
	if (!ast || !ctx)
		return (1);
	if (ast->type == NODE_CMD)
		return (execute_simple_command(ast, ctx));
	if (ast->type == NODE_PIPE)
		return (execute_pipe(ast, ctx));
	if (ast->type == NODE_AND)
		return (execute_and(ast, ctx));
	if (ast->type == NODE_OR)
		return (execute_or(ast, ctx));
	if (ast->type == NODE_SUBSHELL)
		return (execute_subshell(ast, ctx));
	if (ast->type == NODE_SEQ)
		return (execute_seq(ast, ctx));
	return (1);
}
