/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*remove_quotes(char *str)
{
	int		len;
	char	*result;

	if (!str || !*str)
		return (ft_strdup(""));
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

int	get_var_name_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*get_var_value(char *var_name, t_shell_context *ctx)
{
	char	*value;

	if (!var_name || !*var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(ctx->last_exit_code));
	value = env_get(var_name, ctx->env);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*append_string(char *result, char *to_add)
{
	char	*new_result;

	if (!to_add)
		return (result);
	if (!result)
		return (ft_strdup(to_add));
	new_result = ft_strjoin(result, to_add);
	free(result);
	return (new_result);
}

char	*append_char(char *result, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_string(result, tmp));
}
