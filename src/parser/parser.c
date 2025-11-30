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

t_ast_node	*parse_seq(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_or(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_and(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_pipe(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_primary(t_token **tokens, int *pos, t_shell_context *ctx);

t_ast_node	*parse(t_token **tokens, t_shell_context *ctx)
{
	int	pos;

	pos = 0;
	if (!tokens || !tokens[0])
		return (NULL);
	return (parse_seq(tokens, &pos, ctx));
}

t_ast_node	*parse_seq(t_token **tokens, int *pos, t_shell_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_or(tokens, pos, ctx);
	while (tokens[*pos] && tokens[*pos]->type == TOKEN_SEMICOLON)
	{
		(*pos)++;
		if (!tokens[*pos] || tokens[*pos]->type == TOKEN_SEMICOLON)
			break ;
		node = ast_node_create(NODE_SEQ);
		if (!node)
		{
			ast_node_free(left);
			return (NULL);
		}
		node->left = left;
		node->right = parse_or(tokens, pos, ctx);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

t_ast_node	*parse_or(t_token **tokens, int *pos, t_shell_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_and(tokens, pos, ctx);
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
		node->right = parse_and(tokens, pos, ctx);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

t_ast_node	*parse_and(t_token **tokens, int *pos, t_shell_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_pipe(tokens, pos, ctx);
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
		node->right = parse_pipe(tokens, pos, ctx);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

t_ast_node	*parse_pipe(t_token **tokens, int *pos, t_shell_context *ctx)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_primary(tokens, pos, ctx);
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
		node->right = parse_primary(tokens, pos, ctx);
		if (!node->right)
		{
			ast_node_free(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}
