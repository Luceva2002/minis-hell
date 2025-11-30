/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                            ECHO BUILTIN                                   */
/* ========================================================================== */

/**
 * is_n_flag - Verifica se un argomento è un flag -n valido
 * @arg: L'argomento da verificare
 * 
 * In bash, -n, -nn, -nnn, -nnnn sono tutti validi e equivalenti.
 * Un flag -n valido inizia con '-' seguito da una o più 'n'.
 * 
 * Return: 1 se è un flag -n valido, 0 altrimenti
 */
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * builtin_echo - Implementa il comando echo
 * @args: Array di argomenti (args[0] = "echo")
 * 
 * Comportamento:
 * - Stampa gli argomenti separati da spazi
 * - Flag -n: non stampa newline finale
 * - Può avere multipli -n consecutivi (bash behavior)
 * - -n, -nn, -nnn sono tutti equivalenti a -n
 * 
 * Return: Sempre 0 (echo non fallisce mai)
 */
int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
