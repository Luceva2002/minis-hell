/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_substring(const char *s, int start, int len)
{
	char	*result;
	int		i;

	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[len] = '\0';
	return (result);
}

char	*append_to_result(char *result, const char *s, int start, int len)
{
	char	*part;
	char	*new_result;

	part = extract_substring(s, start, len);
	if (!part)
		return (result);
	if (!result)
		return (part);
	new_result = ft_strjoin(result, part);
	free(result);
	free(part);
	return (new_result);
}

int	is_word_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')' || c == ';' || c == '\0');
}
