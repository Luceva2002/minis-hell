/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_primary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*parse_or(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_redir_and_cmd(t_token **tokens, int *pos,
				t_shell_context *ctx);

static t_ast_node	*parse_subshell(t_token **tokens, int *pos,
					t_shell_context *ctx)
{
	t_ast_node	*node;

	(*pos)++;
	node = ast_node_create(NODE_SUBSHELL);
	if (!node)
		return (NULL);
	node->left = parse_or(tokens, pos, ctx);
	if (!node->left || !tokens[*pos]
		|| tokens[*pos]->type != TOKEN_RPAREN)
	{
		ast_node_free(node);
		return (NULL);
	}
	(*pos)++;
	return (node);
}

t_ast_node	*parse_primary(t_token **tokens, int *pos, t_shell_context *ctx)
{
	if (!tokens[*pos])
		return (NULL);
	if (tokens[*pos]->type == TOKEN_LPAREN)
		return (parse_subshell(tokens, pos, ctx));
	return (parse_redir_and_cmd(tokens, pos, ctx));
}

