/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:45:40 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

 
 
 

 
t_token	*token_create(t_token_type type, const char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = ft_strdup(value);
	tok->content = NULL;
	tok->pos = 0;
	tok->next = NULL;
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	return (tok);
}

 
 
 

 
static int	token_array_count(t_token **tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens[count])
		count++;
	return (count);
}

 
int	token_array_add(t_token ***tokens, t_token *tok)
{
	t_token	**new_arr;
	int		count;
	int		i;

	count = token_array_count(*tokens);
	new_arr = malloc(sizeof(t_token *) * (count + 2));
	if (!new_arr)
		return (0);
	i = 0;
	while (i < count)
	{
		new_arr[i] = (*tokens)[i];
		i++;
	}
	new_arr[count] = tok;
	new_arr[count + 1] = NULL;
	if (*tokens)
		free(*tokens);
	*tokens = new_arr;
	return (1);
}

 
 
 

 
void	token_array_free(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->value)
			free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
