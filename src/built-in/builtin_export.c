/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 16:47:02 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_sorted_env(t_shell_context *ctx)
{
	t_env	*env;

	env = ctx->env;
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		if (env->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		env = env->next;
	}
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	set_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static int	export_single_var(char *arg, t_shell_context *ctx)
{
	char	*key;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	set_key_value(arg, &key, &value);
	env_set(&ctx->env, key, value);
	free(key);
	free(value);
	return (0);
}

int	builtin_export(char **args, t_shell_context *ctx)
{
	int	i;
	int	exit_status;

	if (!args[1])
	{
		print_sorted_env(ctx);
		return (0);
	}
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (export_single_var(args[i], ctx) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
