/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

static void	execute_in_child(t_command *cmd, char *path, t_shell_context *ctx)
{
	char	**envp;

	setup_signals_child();
	if (cmd->redirect && !apply_redirections(cmd->redirect))
		exit(1);
	envp = env_to_array(ctx->env);
	if (!envp)
	{
		ft_putstr_fd("minishell: malloc error\n", 2);
		exit(1);
	}
	execve(path, cmd->args, envp);
	perror("minishell");
	exit(127);
}

static void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

static int	handle_ext_signal(int status)
{
	if (WTERMSIG(status) == SIGINT)
		return (write(1, "\n", 1), 130);
	if (WTERMSIG(status) == SIGQUIT)
		return (ft_putendl_fd("Quit: 3", 2), 131);
	return (128 + WTERMSIG(status));
}

int	execute_external_command(t_command *cmd, t_shell_context *ctx)
{
	char	*path;
	pid_t	pid;
	int		status;

	path = find_executable_in_path(cmd->args[0], ctx->env);
	if (!path)
		return (cmd_not_found(cmd->args[0]), 127);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), free(path), 1);
	if (pid == 0)
		execute_in_child(cmd, path, ctx);
	free(path);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		return (handle_ext_signal(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
