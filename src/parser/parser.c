/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*parse_or(t_token **tokens, int *pos);
t_ast_node	*parse_and(t_token **tokens, int *pos);
t_ast_node	*parse_pipe(t_token **tokens, int *pos);
t_ast_node	*parse_primary(t_token **tokens, int *pos);

t_ast_node	*parse(t_token **tokens)
{
	int	pos;

	pos = 0;
	if (!tokens || !tokens[0])
		return (NULL);
	return (parse_or(tokens, &pos));
}

t_ast_node	*parse_or(t_token **tokens, int *pos)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_and(tokens, pos);
	while (tokens[*pos] && tokens[*pos]->type == TOKEN_OR)
	{
		(*pos)++;
		node = ast_node_create(NODE_OR);
		if (!node)
		{
			ast_node_free(left);
			return (NULL);
		}
		node->left = left;
		node->right = parse_and(tokens, pos);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

t_ast_node	*parse_and(t_token **tokens, int *pos)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_pipe(tokens, pos);
	while (tokens[*pos] && tokens[*pos]->type == TOKEN_AND)
	{
		(*pos)++;
		node = ast_node_create(NODE_AND);
		if (!node)
		{
			ast_node_free(left);
			return (NULL);
		}
		node->left = left;
		node->right = parse_pipe(tokens, pos);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

t_ast_node	*parse_pipe(t_token **tokens, int *pos)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_primary(tokens, pos);
	while (tokens[*pos] && tokens[*pos]->type == TOKEN_PIPE)
	{
		(*pos)++;
		node = ast_node_create(NODE_PIPE);
		if (!node)
		{
			ast_node_free(left);
			return (NULL);
		}
		node->left = left;
		node->right = parse_primary(tokens, pos);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}
