/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		get_var_name_len(char *str);
char	*get_var_value(char *var_name, t_shell_context *ctx);
char	*append_string(char *result, char *to_add);
char	*append_char(char *result, char c);

static void	expand_var(char *str, int *i, char **result, t_shell_context *ctx)
{
	int		var_len;
	char	*var_name;
	char	*var_value;

	var_len = get_var_name_len(&str[*i + 1]);
	var_name = ft_substr(str, *i + 1, var_len);
	var_value = get_var_value(var_name, ctx);
	*result = append_string(*result, var_value);
	free(var_name);
	free(var_value);
	*i += var_len + 1;
}

static int	is_expandable_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1]
		&& (ft_isalpha(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'));
}

char	*expand_variables(char *str, t_shell_context *ctx, int in_quotes)
{
	char	*result;
	int		i;

	(void)in_quotes;
	if (!str)
		return (NULL);
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (is_expandable_var(str, i))
			expand_var(str, &i, &result, ctx);
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}
