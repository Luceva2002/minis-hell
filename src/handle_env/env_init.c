/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	env_add_var(t_env **env, char *envp_entry)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(envp_entry, '=');
	if (!equal)
		return (1);
	key = ft_substr(envp_entry, 0, equal - envp_entry);
	value = ft_strdup(equal + 1);
	if (!key || !value || !env_set(env, key, value))
	{
		free(key);
		free(value);
		return (0);
	}
	free(key);
	free(value);
	return (1);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;

	if (!envp)
		return (NULL);
	env = NULL;
	i = 0;
	while (envp[i])
	{
		if (!env_add_var(&env, envp[i]))
			return (env_free(env), NULL);
		i++;
	}
	return (env);
}

void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
