/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:40:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*ast_node_create(t_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}

t_command	*cmd_node_create(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->comm = NULL;
	cmd->args = NULL;
	cmd->redirect = NULL;
	return (cmd);
}

static int	count_arguments(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

int	cmd_add_argument(t_command *cmd, char *arg)
{
	int		i;
	int		count;
	char	**new_args;

	if (!cmd || !arg)
		return (0);
	count = count_arguments(cmd->args);
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = -1;
	while (++i < count)
		new_args[i] = cmd->args[i];
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
		return (free(new_args), 0);
	new_args[i + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	if (!cmd->comm)
		cmd->comm = ft_strdup(arg);
	return (1);
}
