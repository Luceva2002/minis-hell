/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:40:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_arguments(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

static void	free_redirects(t_redirect **redirect)
{
	int	i;

	if (!redirect)
		return ;
	i = 0;
	while (redirect[i])
	{
		if (redirect[i]->filename)
			free(redirect[i]->filename);
		free(redirect[i]);
		i++;
	}
	free(redirect);
}

static void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->comm)
		free(cmd->comm);
	free_arguments(cmd->args);
	free_redirects(cmd->redirect);
	free(cmd);
}

void	ast_node_free(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->left)
		ast_node_free(node->left);
	if (node->right)
		ast_node_free(node->right);
	if (node->cmd)
		free_command(node->cmd);
	free(node);
}
