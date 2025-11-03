/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:30:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                    PARENTHESES BALANCE VALIDATION                         */
/* ========================================================================== */

/**
 * syntax_check_parens - Verifica il bilanciamento delle parentesi
 * @tokens: Array di token da verificare
 * 
 * Usa un contatore di profondità (depth) per tracciare l'annidamento:
 * - Ogni '(' incrementa la profondità
 * - Ogni ')' decrementa la profondità
 * - Se la profondità diventa negativa, c'è una ')' senza '(' corrispondente
 * - Se alla fine la profondità non è 0, ci sono '(' non chiuse
 * 
 * Return: SYNTAX_OK se bilanciate, SYNTAX_PARENS altrimenti
 */
int	syntax_check_parens(t_token **tokens)
{
	int	i;
	int	depth;

	i = 0;
	depth = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == TOKEN_LPAREN)
			depth++;
		else if (tokens[i]->type == TOKEN_RPAREN)
		{
			depth--;
			if (depth < 0)
				return (SYNTAX_PARENS);
		}
		i++;
	}
	if (depth != 0)
		return (SYNTAX_PARENS);
	return (SYNTAX_OK);
}
