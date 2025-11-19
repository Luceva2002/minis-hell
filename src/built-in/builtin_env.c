/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/09 00:55:08 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                            ENV BUILTIN                                    */
/* ========================================================================== */

/**
 * builtin_env - Implementa il comando env
 * @ctx: Contesto della shell
 * 
 * Stampa tutte le variabili d'ambiente nel formato KEY=value.
 * Non accetta opzioni o argomenti (a differenza di export).
 * 
 * Return: 0 in caso di successo
 */
int	builtin_env(t_shell_context *ctx)
{
	t_env	*env;

	env = ctx->env;
	while (env)
	{
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putendl_fd(env->value, STDOUT_FILENO);
		env = env->next;
	}
	return (0);
}

