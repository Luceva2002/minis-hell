/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                           UNSET BUILTIN                                   */
/* ========================================================================== */

/**
 * builtin_unset - Implementa il comando unset
 * @args: Array di argomenti (args[0] = "unset", args[1..n] = variabili)
 * @ctx: Contesto della shell
 * 
 * Rimuove le variabili specificate dall'environment.
 * Non restituisce errore se una variabile non esiste.
 * 
 * Return: Sempre 0 (unset non fallisce)
 */
int	builtin_unset(char **args, t_shell_context *ctx)
{
	int	i;

	i = 1;
	while (args[i])
	{
		env_unset(&ctx->env, args[i]);
		i++;
	}
	return (0);
}


