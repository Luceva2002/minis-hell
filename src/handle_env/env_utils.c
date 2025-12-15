/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 16:49:29 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*env_get(char *key, t_env *env)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static t_env	*env_find(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static int	env_set_existing(t_env *node, char *value)
{
	free(node->value);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	return (1);
}

static int	env_set_new(t_env **env, char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	if (!new->key)
		return (free(new), 0);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = *env;
	*env = new;
	return (1);
}

int	env_set(t_env **env, char *key, char *value)
{
	t_env	*node;

	if (!env || !key)
		return (0);
	node = env_find(*env, key);
	if (node)
		return (env_set_existing(node, value));
	return (env_set_new(env, key, value));
}
