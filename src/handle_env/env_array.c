/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_unset(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !key)
		return (0);
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	free_envp_partial(char **envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	char	*tmp;
	int		i;

	envp = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		if (!tmp)
			return (free_envp_partial(envp, i), NULL);
		envp[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		if (!envp[i])
			return (free_envp_partial(envp, i), NULL);
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
