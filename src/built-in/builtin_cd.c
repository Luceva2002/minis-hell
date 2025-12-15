/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_target_directory(char **args, t_shell_context *ctx)
{
	char	*target;

	if (!args[1])
	{
		target = env_get("HOME", ctx->env);
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (target);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		target = env_get("OLDPWD", ctx->env);
		if (!target)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putendl_fd(target, STDOUT_FILENO);
		return (target);
	}
	return (args[1]);
}

static void	update_pwd_vars(char *old_pwd, t_shell_context *ctx)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("getcwd");
		free(old_pwd);
		return ;
	}
	env_set(&ctx->env, "OLDPWD", old_pwd);
	env_set(&ctx->env, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
}

int	builtin_cd(char **args, t_shell_context *ctx)
{
	char	*target;
	char	*old_pwd;

	target = get_target_directory(args, ctx);
	if (!target)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target);
		free(old_pwd);
		return (1);
	}
	update_pwd_vars(old_pwd, ctx);
	return (0);
}
