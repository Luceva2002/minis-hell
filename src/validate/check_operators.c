/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:30:00 by luevange         ###   ########.fr       */
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
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
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
 * syntax_check_operators - Valida la sequenza di operatori
 * @tokens: Array di token da validare
 * 
 * Verifica che:
 * 1. Non ci siano operatori di controllo (pipe, &&, ||) all'inizio
 * 2. Non ci siano operatori alla fine senza comando dopo
 * 3. Gli operatori di redirect abbiano sempre un argomento dopo
 * 4. Non ci siano sequenze invalide di operatori
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
			if (i == 0 && tokens[i]->type != TOKEN_REDIR_IN
				&& tokens[i]->type != TOKEN_REDIR_OUT
				&& tokens[i]->type != TOKEN_APPEND
				&& tokens[i]->type != TOKEN_HEREDOC)
				return (SYNTAX_OPERATOR);
			if (!tokens[i + 1])
				return (SYNTAX_OPERATOR);
			if (needs_argument(tokens[i]->type))
			{
				if (tokens[i + 1]->type != TOKEN_WORD)
					return (SYNTAX_OPERATOR);
			}
		}
		i++;
	}
	return (SYNTAX_OK);
}
