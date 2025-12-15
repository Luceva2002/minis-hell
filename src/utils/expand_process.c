/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables(char *str, t_shell_context *ctx, int in_quotes);

static char	*process_quoted_segment(char *value, int *i, t_shell_context *ctx)
{
	char	quote;
	int		start;
	char	*content;
	char	*result;

	quote = value[*i];
	(*i)++;
	start = *i;
	while (value[*i] && value[*i] != quote)
		(*i)++;
	content = ft_substr(value, start, *i - start);
	if (value[*i] == quote)
		(*i)++;
	if (!content)
		return (ft_strdup(""));
	if (quote == '\'')
		return (content);
	result = expand_variables(content, ctx, 1);
	free(content);
	return (result);
}

static char	*process_unquoted_segment(char *value, int *i, t_shell_context *ctx)
{
	int		start;
	char	*content;
	char	*result;

	start = *i;
	while (value[*i] && value[*i] != '\'' && value[*i] != '"')
		(*i)++;
	content = ft_substr(value, start, *i - start);
	if (!content)
		return (ft_strdup(""));
	result = expand_variables(content, ctx, 0);
	free(content);
	return (result);
}

static char	*join_segment(char *result, char *segment)
{
	char	*new_result;

	if (!segment)
		return (result);
	new_result = ft_strjoin(result, segment);
	free(result);
	free(segment);
	return (new_result);
}

char	*process_token_value(char *value, t_shell_context *ctx)
{
	char	*result;
	char	*segment;
	int		i;

	if (!value || !*value)
		return (ft_strdup(""));
	result = ft_strdup("");
	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			segment = process_quoted_segment(value, &i, ctx);
		else
			segment = process_unquoted_segment(value, &i, ctx);
		result = join_segment(result, segment);
		if (!result)
			return (ft_strdup(""));
	}
	return (result);
}
