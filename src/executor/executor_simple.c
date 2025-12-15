/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_builtin_redir(t_command *cmd, t_shell_context *ctx)
{
	pid_t	pid;
	int		status;

	if (!cmd->redirect)
		return (execute_builtin(cmd, ctx));
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		if (!apply_redirections(cmd->redirect))
			exit(1);
		exit(execute_builtin(cmd, ctx));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static void	compact_args(char **args)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (args[++i])
	{
		if (args[i][0])
			args[j++] = args[i];
		else
			free(args[i]);
	}
	args[j] = NULL;
}

static int	expand_args(t_command *cmd, t_shell_context *ctx)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->args)
		return (1);
	i = -1;
	while (cmd->args[++i])
	{
		expanded = process_token_value(cmd->args[i], ctx);
		if (!expanded)
			return (0);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
	}
	compact_args(cmd->args);
	return (1);
}

static int	expand_redirects(t_command *cmd, t_shell_context *ctx)
{
	int		i;
	char	*expanded;

	if (!cmd->redirect)
		return (1);
	i = 0;
	while (cmd->redirect[i])
	{
		if (cmd->redirect[i]->status != 3)
		{
			expanded = process_token_value(cmd->redirect[i]->filename, ctx);
			if (!expanded)
				return (0);
			free(cmd->redirect[i]->filename);
			cmd->redirect[i]->filename = expanded;
		}
		i++;
	}
	return (1);
}

int	execute_simple_command(t_ast_node *node, t_shell_context *ctx)
{
	if (!node || !node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (0);
	if (!expand_args(node->cmd, ctx) || !expand_redirects(node->cmd, ctx))
		return (1);
	if (!node->cmd->args[0] || !node->cmd->args[0][0])
		return (0);
	if (is_builtin(node->cmd->args[0]))
		return (execute_builtin_redir(node->cmd, ctx));
	return (execute_external_command(node->cmd, ctx));
}
