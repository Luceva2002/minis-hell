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
