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

/* ========================================================================== */
/*                       MEMORY CLEANUP FUNCTIONS                            */
/* ========================================================================== */

/**
 * free_arguments - Libera un array di argomenti
 * @args: Array di stringhe da liberare (NULL-terminated)
 */
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

/**
 * free_redirects - Libera un array di redirezioni
 * @redirect: Array di redirezioni da liberare (NULL-terminated)
 */
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

/**
 * free_command - Libera un nodo comando e i suoi contenuti
 * @cmd: Il comando da liberare
 * 
 * Libera ricorsivamente tutti i campi del comando:
 * - Il nome del comando
 * - L'array di argomenti
 * - L'array di redirezioni
 */
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

/**
 * ast_node_free - Libera ricorsivamente un nodo AST e i suoi figli
 * @node: Il nodo da liberare
 * 
 * Attraversa l'AST in post-order (prima i figli, poi il nodo corrente)
 * liberando tutta la memoria allocata.
 */
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
