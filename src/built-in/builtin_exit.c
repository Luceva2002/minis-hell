/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                          EXIT HELPERS                                     */
/* ========================================================================== */

/**
 * is_numeric - Verifica se una stringa è un numero valido
 * @str: Stringa da verificare
 * 
 * Accetta numeri con segno opzionale (+/-) seguito da cifre.
 * 
 * Return: 1 se numerico, 0 altrimenti
 */
static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* ========================================================================== */
/*                           EXIT BUILTIN                                    */
/* ========================================================================== */

/**
 * builtin_exit - Implementa il comando exit
 * @args: Array di argomenti
 * @ctx: Contesto della shell
 * 
 * Comportamento:
 * - Stampa "exit"
 * - Senza args: esce con last_exit_code
 * - Con arg numerico: esce con quel codice (modulo 256)
 * - Con arg non numerico: errore ed esce con 2
 * - Con troppi args: errore ma NON esce, return 1
 * 
 * Return: Solo se troppi argomenti (return 1), altrimenti chiama exit()
 */
int	builtin_exit(char **args, t_shell_context *ctx)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit(ctx->last_exit_code);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit(exit_code % 256);
}


