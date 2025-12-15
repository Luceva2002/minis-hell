/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(t_token_type type);
int	is_control_operator(t_token_type type);
int	needs_argument(t_token_type type);
int	is_after_open_context(t_token **tokens, int i);

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

static int	check_single_operator(t_token **tokens, int i)
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
	return (SYNTAX_OK);
}

int	syntax_check_operators(t_token **tokens)
{
	int	i;
	int	result;

	i = 0;
	while (tokens[i])
	{
		if (is_operator(tokens[i]->type))
		{
			result = check_single_operator(tokens, i);
			if (result != SYNTAX_OK)
				return (result);
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
