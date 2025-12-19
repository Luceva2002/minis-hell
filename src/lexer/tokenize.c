/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/30 21:24:44 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	char_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	char_is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')' || c == ';');
}

static t_token	*get_next_token(const char *s, int *i)
{
	t_token_type	type;
	char			*value;
	t_token			*tok;

	while (s[*i] && char_is_space(s[*i]))
		(*i)++;
	if (!s[*i])
		return (NULL);
	if (char_is_operator(s[*i]))
	{
		value = read_operator(s, i, &type);
		if (!value)
			return (NULL);
		tok = token_create(type, value);
		free(value);
		return (tok);
	}
	value = read_word(s, i);
	if (!value)
		return (NULL);
	tok = token_create(TOKEN_WORD, value);
	free(value);
	return (tok);
}

t_token	**tokenize(const char *input)
{
	t_token	**tokens;
	t_token	*tok;
	int		i;

	tokens = malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	tokens[0] = NULL;
	i = 0;
	while (input[i])
	{
		tok = get_next_token(input, &i);
		if (!tok)
			break ;
		if (!token_array_add(&tokens, tok))
		{
			token_array_free(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
