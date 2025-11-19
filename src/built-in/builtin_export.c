/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                         EXPORT WITHOUT ARGS                               */
/* ========================================================================== */

/**
 * print_sorted_env - Stampa environment ordinato (come export senza args)
 * @ctx: Contesto della shell
 * 
 * Per semplicità, stampiamo senza ordinamento (bash ordina alfabeticamente).
 * Formato: declare -x KEY="value"
 */
static void	print_sorted_env(t_shell_context *ctx)
{
	t_env	*env;

	env = ctx->env;
	while (env)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(env->value, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
		env = env->next;
	}
}

/* ========================================================================== */
/*                        EXPORT VALIDATION                                  */
/* ========================================================================== */

/**
 * is_valid_identifier - Verifica se un nome variabile è valido
 * @str: Nome da verificare
 * 
 * Un identificatore valido:
 * - Inizia con lettera o underscore
 * - Seguito da lettere, numeri o underscore
 * 
 * Return: 1 se valido, 0 altrimenti
 */
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

/* ========================================================================== */
/*                       EXPORT WITH ARGUMENTS                               */
/* ========================================================================== */

/**
 * export_single_var - Esporta una singola variabile
 * @arg: Argomento nel formato KEY=value o solo KEY
 * @ctx: Contesto della shell
 * 
 * Return: 0 in caso di successo, 1 in caso di errore
 */
static int	export_single_var(char *arg, t_shell_context *ctx)
{
	char	*equal;
	char	*key;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = ft_substr(arg, 0, equal - arg);
		value = ft_strdup(equal + 1);
		env_set(&ctx->env, key, value);
		free(key);
		free(value);
	}
	return (0);
}

/**
 * builtin_export - Implementa il comando export
 * @args: Array di argomenti
 * @ctx: Contesto della shell
 * 
 * Senza argomenti: stampa tutte le variabili
 * Con argomenti: imposta le variabili specificate
 * 
 * Return: 0 in caso di successo, 1 se ci sono identificatori invalidi
 */
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


