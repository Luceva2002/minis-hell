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

/* ========================================================================== */
/*                            CD BUILTIN                                     */
/* ========================================================================== */

/**
 * get_target_directory - Determina la directory target per cd
 * @args: Array di argomenti
 * @ctx: Contesto della shell
 * 
 * - Senza argomenti: va a HOME
 * - Con "-": va a OLDPWD
 * - Con path: usa il path specificato
 * 
 * Return: Path della directory target, NULL se HOME/OLDPWD non sono settati
 */
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

/**
 * update_pwd_vars - Aggiorna PWD e OLDPWD nell'environment
 * @old_pwd: Vecchia directory di lavoro
 * @ctx: Contesto della shell
 */
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

/**
 * builtin_cd - Implementa il comando cd
 * @args: Array di argomenti (args[0] = "cd", args[1] = target)
 * @ctx: Contesto della shell
 * 
 * Cambia la directory corrente e aggiorna PWD e OLDPWD.
 * bash ignora argomenti extra, ma alcuni test potrebbero verificarlo.
 * 
 * Return: 0 in caso di successo, 1 in caso di errore
 */
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


