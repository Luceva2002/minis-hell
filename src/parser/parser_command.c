/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/16 00:49:48 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirect_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	get_redir_status(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (0);
	else if (type == TOKEN_REDIR_OUT)
		return (1);
	else if (type == TOKEN_APPEND)
		return (2);
	else if (type == TOKEN_HEREDOC)
		return (3);
	return (-1);
}

static int	handle_redirect(t_token **tokens, int *pos, t_command *cmd,
				t_shell_context *ctx)
{
	t_redirect	*redir;
	char		*processed;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (0);
	redir->status = get_redir_status(tokens[*pos]->type);
	(*pos)++;
	if (!tokens[*pos] || tokens[*pos]->type != TOKEN_WORD)
		return (free(redir), 0);
	processed = process_token_value(tokens[*pos]->value, ctx);
	redir->filename = processed;
	if (!redir->filename)
		return (free(redir), 0);
	if (!cmd_add_redirect(cmd, redir))
		return (free(redir->filename), free(redir), 0);
	(*pos)++;
	return (1);
}

static int	process_token(t_token **tokens, int *pos, t_command *cmd,
				t_shell_context *ctx)
{
	char	*processed;

	if (tokens[*pos]->type == TOKEN_WORD)
	{
		processed = process_token_value(tokens[*pos]->value, ctx);
		if (!processed || !cmd_add_argument(cmd, processed))
		{
			free(processed);
			return (0);
		}
		free(processed);
		(*pos)++;
	}
	else
	{
		if (!handle_redirect(tokens, pos, cmd, ctx))
			return (0);
	}
	return (1);
}

t_ast_node	*parse_redir_and_cmd(t_token **tokens, int *pos,
				t_shell_context *ctx)
{
	t_ast_node	*node;

	node = ast_node_create(NODE_CMD);
	if (!node)
		return (NULL);
	node->cmd = cmd_node_create();
	if (!node->cmd)
		return (free(node), NULL);
	while (tokens[*pos] && (tokens[*pos]->type == TOKEN_WORD
			|| is_redirect_token(tokens[*pos]->type)))
	{
		if (!process_token(tokens, pos, node->cmd, ctx))
			return (ast_node_free(node), NULL);
	}
	return (node);
}
