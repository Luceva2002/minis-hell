/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_logic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_and(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	if (exit_status == 0)
		exit_status = execute(node->right, ctx);
	return (exit_status);
}

int	execute_or(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	if (exit_status != 0)
		exit_status = execute(node->right, ctx);
	return (exit_status);
}
