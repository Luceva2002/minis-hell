/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/30 21:24:44 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                      OPERATOR TYPE CHECKERS                               */
/* ========================================================================== */

/**
 * is_operator - Verifica se un token è un operatore
 * @type: Il tipo del token da verificare
 * 
 * Return: 1 se è un operatore, 0 altrimenti
 */
static int	is_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC
		|| type == TOKEN_SEMICOLON);
}

/**
 * is_control_operator - Verifica se è un operatore di controllo (|, &&, ||, ;)
 * @type: Il tipo del token da verificare
 * 
 * Return: 1 se è un operatore di controllo, 0 altrimenti
 */
static int	is_control_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_SEMICOLON);
}

/**
 * needs_argument - Verifica se un operatore richiede un argomento dopo
 * @type: Il tipo del token da verificare
 * 
 * Gli operatori di redirect (<, >, <<, >>) richiedono sempre un filename
 * o delimiter dopo di loro.
 * 
 * Return: 1 se richiede un argomento, 0 altrimenti
 */
static int	needs_argument(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

/* ========================================================================== */
/*                    OPERATOR SEQUENCE VALIDATION                           */
/* ========================================================================== */

/**
 * is_after_open_context - Verifica se siamo all'inizio di un contesto
 * @tokens: Array di token
 * @i: Posizione corrente
 * 
 * Return: 1 se siamo all'inizio o dopo '(' o dopo un operatore di controllo
 */
static int	is_after_open_context(t_token **tokens, int i)
{
	if (i == 0)
		return (1);
	if (tokens[i - 1]->type == TOKEN_LPAREN)
		return (1);
	return (0);
}

/**
 * is_valid_after_semicolon - Verifica se il token dopo ; è valido
 * @tokens: Array di token
 * @i: Posizione del ;
 * 
 * Il ; può essere seguito da nulla (fine input), da ) o da un comando
 * Ma non può essere seguito da altri operatori di controllo
 */
static int	check_after_control_op(t_token **tokens, int i)
{
	t_token_type	next_type;

	if (!tokens[i + 1])
	{
		if (tokens[i]->type == TOKEN_SEMICOLON)
			return (1);
		return (0);
	}
	next_type = tokens[i + 1]->type;
	if (tokens[i]->type == TOKEN_SEMICOLON)
	{
		if (next_type == TOKEN_RPAREN || next_type == TOKEN_WORD
			|| next_type == TOKEN_LPAREN || needs_argument(next_type))
			return (1);
		if (next_type == TOKEN_SEMICOLON)
			return (1);
		return (0);
	}
	if (is_control_operator(next_type) || next_type == TOKEN_RPAREN)
		return (0);
	return (1);
}

/**
 * syntax_check_operators - Valida la sequenza di operatori
 * @tokens: Array di token da validare
 * 
 * Verifica che:
 * 1. Non ci siano operatori di controllo (pipe, &&, ||) all'inizio o dopo (
 * 2. Non ci siano operatori alla fine senza comando dopo (eccetto ;)
 * 3. Gli operatori di redirect abbiano sempre un argomento dopo
 * 4. Non ci siano sequenze invalide di operatori (es: | |, && ||)
 * 
 * Return: SYNTAX_OK se valido, SYNTAX_OPERATOR altrimenti
 */
int	syntax_check_operators(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_operator(tokens[i]->type))
		{
			if (is_after_open_context(tokens, i)
				&& is_control_operator(tokens[i]->type))
				return (SYNTAX_OPERATOR);
			if (needs_argument(tokens[i]->type))
			{
				if (!tokens[i + 1] || tokens[i + 1]->type != TOKEN_WORD)
					return (SYNTAX_OPERATOR);
			}
			else if (is_control_operator(tokens[i]->type))
			{
				if (!check_after_control_op(tokens, i))
					return (SYNTAX_OPERATOR);
			}
		}
		else if (tokens[i]->type == TOKEN_LPAREN)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == TOKEN_RPAREN)
				return (SYNTAX_OPERATOR);
		}
		i++;
	}
	return (SYNTAX_OK);
}
