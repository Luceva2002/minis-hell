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
 * ft_isspace - Verifica se un carattere è uno spazio bianco
 * @c: Carattere da verificare
 * 
 * Return: 1 se è spazio, 0 altrimenti
 */
static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

/**
 * ft_atol - Converte una stringa in long long
 * @str: Stringa da convertire
 * 
 * Return: Valore numerico della stringa
 */
static long long	ft_atol(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

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
	while (ft_isspace(str[i]))
		i++;
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
 * Comportamento (come bash):
 * - Stampa "exit"
 * - Senza args: esce con last_exit_code
 * - Con arg numerico: esce con quel codice (modulo 256, gestisce negativi)
 * - Con arg non numerico: errore ed esce con 2
 * - Con troppi args: errore ma NON esce, return 1
 * 
 * Note: I codici negativi vengono convertiti come in bash:
 * exit -1 -> 255, exit -2 -> 254, etc.
 * 
 * Return: Solo se troppi argomenti (return 1), altrimenti chiama exit()
 */
int	builtin_exit(char **args, t_shell_context *ctx)
{
	long long		exit_code;
	unsigned char	final_code;

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
	exit_code = ft_atol(args[1]);
	final_code = (unsigned char)(exit_code & 0xFF);
	exit(final_code);
}


