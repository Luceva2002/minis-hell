/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_to_result(char *result, const char *s, int start, int len);
int		is_word_delimiter(char c);

char	*read_quoted(const char *s, int *i)
{
	char	quote;
	int		start;
	int		j;
	char	*result;

	quote = s[*i];
	start = *i;
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
	result = malloc((*i - start) + 1);
	if (!result)
		return (NULL);
	j = 0;
	while (j < (*i - start))
	{
		result[j] = s[start + j];
		j++;
	}
	result[j] = '\0';
	return (result);
}

static void	set_operator_type(const char *val, t_token_type *type)
{
	if (val[0] == '|' && val[1] == '|')
		*type = TOKEN_OR;
	else if (val[0] == '|')
		*type = TOKEN_PIPE;
	else if (val[0] == '&' && val[1] == '&')
		*type = TOKEN_AND;
	else if (val[0] == '<' && val[1] == '<')
		*type = TOKEN_HEREDOC;
	else if (val[0] == '<')
		*type = TOKEN_REDIR_IN;
	else if (val[0] == '>' && val[1] == '>')
		*type = TOKEN_APPEND;
	else if (val[0] == '>')
		*type = TOKEN_REDIR_OUT;
	else if (val[0] == '(')
		*type = TOKEN_LPAREN;
	else if (val[0] == ')')
		*type = TOKEN_RPAREN;
	else if (val[0] == ';')
		*type = TOKEN_SEMICOLON;
}

char	*read_operator(const char *s, int *i, t_token_type *type)
{
	int		start;
	char	*val;

	start = *i;
	if ((s[*i] == '|' && s[*i + 1] == '|')
		|| (s[*i] == '&' && s[*i + 1] == '&')
		|| (s[*i] == '<' && s[*i + 1] == '<')
		|| (s[*i] == '>' && s[*i + 1] == '>'))
		(*i) += 2;
	else
		(*i)++;
	val = ft_substr(s, start, *i - start);
	set_operator_type(val, type);
	return (val);
}

static void	handle_quoted_word(const char *s, int *i, char **result)
{
	char	*quoted;
	char	*tmp;

	quoted = read_quoted(s, i);
	if (quoted)
	{
		if (!*result)
			*result = quoted;
		else
		{
			tmp = ft_strjoin(*result, quoted);
			free(*result);
			free(quoted);
			*result = tmp;
		}
	}
}

char	*read_word(const char *s, int *i)
{
	char	*result;
	int		start;

	result = NULL;
	while (s[*i] && !is_word_delimiter(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '"')
			handle_quoted_word(s, i, &result);
		else
		{
			start = *i;
			while (s[*i] && !is_word_delimiter(s[*i])
				&& s[*i] != '\'' && s[*i] != '"')
				(*i)++;
			result = append_to_result(result, s, start, *i - start);
		}
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}
