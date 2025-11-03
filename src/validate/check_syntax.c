/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:30:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                       QUOTE VALIDATION HELPER                             */
/* ========================================================================== */

/**
 * has_unclosed_quote - Verifica se una stringa ha quote non chiuse
 * @str: La stringa da verificare
 * 
 * Scorre la stringa cercando coppie di quote (singole o doppie).
 * Per ogni quote di apertura, cerca la corrispondente di chiusura.
 * 
 * Return: 1 se ci sono quote non chiuse, 0 se tutto è bilanciato
 */
static int	has_unclosed_quote(const char *str)
{
	int		i;
	char	quote;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				i++;
			if (!str[i])
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}

/* ========================================================================== */
/*                     MAIN SYNTAX VALIDATION FUNCTION                       */
/* ========================================================================== */

/**
 * syntax_validate - Valida la sintassi di un array di token
 * @tokens: Array di token da validare
 * 
 * Questa è la funzione principale di validazione. Esegue tutti i controlli
 * sintattici necessari:
 * 1. Verifica che non ci siano quote non chiuse
 * 2. Verifica la corretta sequenza di operatori
 * 3. Verifica il bilanciamento delle parentesi
 * 
 * Return: SYNTAX_OK se tutto è valido, altrimenti un codice di errore
 */
int	syntax_validate(t_token **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (SYNTAX_OK);
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->value && has_unclosed_quote(tokens[i]->value))
			return (SYNTAX_UNCLOSED);
		i++;
	}
	if (syntax_check_operators(tokens) != SYNTAX_OK)
		return (SYNTAX_OPERATOR);
	if (syntax_check_parens(tokens) != SYNTAX_OK)
		return (SYNTAX_PARENS);
	return (SYNTAX_OK);
}
