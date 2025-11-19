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

/* ========================================================================== */
/*                        ENVIRONMENT INITIALIZATION                         */
/* ========================================================================== */

/**
 * env_init - Inizializza l'environment copiando envp
 * @envp: Array di stringhe "KEY=value" dall'ambiente di sistema
 * 
 * Questa funzione crea una lista linked di variabili d'ambiente
 * copiando tutte le variabili da envp. Ogni variabile viene splittata
 * in key e value usando '=' come separatore.
 * 
 * Return: Puntatore alla lista di environment, NULL in caso di errore
 */
t_env	*env_init(char **envp)
{
	t_env	*env;
	char	*equal;
	char	*key;
	char	*value;
	int		i;

	if (!envp)
		return (NULL);
	env = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			key = ft_substr(envp[i], 0, equal - envp[i]);
			value = ft_strdup(equal + 1);
			if (!key || !value || !env_set(&env, key, value))
			{
				free(key);
				free(value);
				return (env_free(env), NULL);
			}
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}

/* ========================================================================== */
/*                          ENVIRONMENT CLEANUP                              */
/* ========================================================================== */

/**
 * env_free - Libera tutta la memoria dell'environment
 * @env: Lista di variabili d'ambiente da liberare
 * 
 * Attraversa la lista liberando key, value e ogni nodo.
 */
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


