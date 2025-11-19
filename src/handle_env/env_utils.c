/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                           ENVIRONMENT GET                                 */
/* ========================================================================== */

/**
 * env_get - Ottiene il valore di una variabile d'ambiente
 * @key: Nome della variabile da cercare
 * @env: Lista delle variabili d'ambiente
 * 
 * Return: Valore della variabile se trovata, NULL altrimenti
 */
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

/* ========================================================================== */
/*                           ENVIRONMENT SET                                 */
/* ========================================================================== */

/**
 * env_find - Cerca un nodo con una determinata key
 * @env: Lista delle variabili d'ambiente
 * @key: Chiave da cercare
 * 
 * Return: Nodo trovato o NULL
 */
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

/**
 * env_set - Imposta o aggiorna una variabile d'ambiente
 * @env: Puntatore alla lista delle variabili
 * @key: Nome della variabile
 * @value: Valore da assegnare
 * 
 * Se la variabile esiste già, aggiorna il valore.
 * Altrimenti crea un nuovo nodo all'inizio della lista.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore
 */
int	env_set(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new;

	if (!env || !key)
		return (0);
	node = env_find(*env, key);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(value);
		if (!node->value)
			return (0);
	}
	else
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (0);
		new->key = ft_strdup(key);
		new->value = ft_strdup(value);
		if (!new->key || !new->value)
			return (free(new->key), free(new->value), free(new), 0);
		new->next = *env;
		*env = new;
	}
	return (1);
}

/* ========================================================================== */
/*                           ENVIRONMENT UNSET                               */
/* ========================================================================== */

/**
 * env_unset - Rimuove una variabile dall'environment
 * @env: Puntatore alla lista delle variabili
 * @key: Nome della variabile da rimuovere
 * 
 * Return: 1 in caso di successo, 0 se non trovata
 */
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

/* ========================================================================== */
/*                      ENVIRONMENT TO ARRAY CONVERSION                      */
/* ========================================================================== */

/**
 * env_count - Conta il numero di variabili nell'environment
 * @env: Lista delle variabili d'ambiente
 * 
 * Return: Numero di variabili
 */
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

/**
 * env_to_array - Converte la lista env in array di stringhe per execve
 * @env: Lista delle variabili d'ambiente
 * 
 * Crea un array di stringhe nel formato "KEY=value" necessario
 * per passare l'environment a execve.
 * 
 * Return: Array NULL-terminated di stringhe, NULL in caso di errore
 */
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
			return (NULL);
		envp[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		if (!envp[i])
			return (NULL);
		env = env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}


