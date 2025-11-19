/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                         MAIN EXECUTOR FUNCTION                            */
/* ========================================================================== */

/**
 * execute - Punto di ingresso dell'executor, traversa l'AST
 * @ast: Nodo AST da eseguire
 * @ctx: Contesto della shell (env, exit status, etc.)
 * 
 * Questa funzione analizza il tipo di nodo AST e chiama la funzione
 * appropriata per eseguirlo. Gestisce tutti i tipi di nodo:
 * - NODE_CMD: comando singolo
 * - NODE_PIPE: pipeline
 * - NODE_AND: operatore logico &&
 * - NODE_OR: operatore logico ||
 * - NODE_SUBSHELL: esecuzione in subshell
 * 
 * Return: Exit status del comando (0-255)
 */
int	execute(t_ast_node *ast, t_shell_context *ctx)
{
	int	exit_status;

	if (!ast || !ctx)
		return (1);
	if (ast->type == NODE_CMD)
		exit_status = execute_simple_command(ast, ctx);
	else if (ast->type == NODE_PIPE)
		exit_status = execute_pipe(ast, ctx);
	else if (ast->type == NODE_AND)
		exit_status = execute_and(ast, ctx);
	else if (ast->type == NODE_OR)
		exit_status = execute_or(ast, ctx);
	else if (ast->type == NODE_SUBSHELL)
	{
		ft_printf("minishell: subshell not yet implemented\n");
		exit_status = 1;
	}
	else
		exit_status = 1;
	return (exit_status);
}

/* ========================================================================== */
/*                      SIMPLE COMMAND EXECUTION                             */
/* ========================================================================== */

/**
 * execute_builtin_with_redirect - Esegue built-in con redirections in subshell
 * @cmd: Comando con redirections
 * @ctx: Contesto della shell
 * 
 * Se ci sono redirections, fa fork per non modificare gli FD della shell principale.
 * Altrimenti esegue direttamente il built-in.
 * 
 * Return: Exit status del built-in
 */
static int	execute_builtin_with_redirect(t_command *cmd, t_shell_context *ctx)
{
	pid_t	pid;
	int		status;

	if (!cmd->redirect)
		return (execute_builtin(cmd, ctx));
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		if (!apply_redirections(cmd->redirect))
			exit(1);
		exit(execute_builtin(cmd, ctx));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * execute_simple_command - Esegue un singolo comando
 * @node: Nodo AST di tipo NODE_CMD
 * @ctx: Contesto della shell
 * 
 * Controlla se il comando è un built-in ed eseguilo direttamente,
 * altrimenti delega a execute_external_command per fork+execve.
 * 
 * Return: Exit status del comando
 */
int	execute_simple_command(t_ast_node *node, t_shell_context *ctx)
{
	if (!node || !node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (0);
	if (is_builtin(node->cmd->args[0]))
		return (execute_builtin_with_redirect(node->cmd, ctx));
	return (execute_external_command(node->cmd, ctx));
}

/* ========================================================================== */
/*                        LOGICAL OPERATORS                                  */
/* ========================================================================== */

/**
 * execute_and - Esegue operatore AND (&&)
 * @node: Nodo AST di tipo NODE_AND
 * @ctx: Contesto della shell
 * 
 * Esegue il comando left. Se ha successo (exit=0), esegue right.
 * Altrimenti ritorna l'exit status di left.
 * 
 * Return: Exit status dell'ultimo comando eseguito
 */
int	execute_and(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	if (exit_status == 0)
		exit_status = execute(node->right, ctx);
	return (exit_status);
}

/**
 * execute_or - Esegue operatore OR (||)
 * @node: Nodo AST di tipo NODE_OR
 * @ctx: Contesto della shell
 * 
 * Esegue il comando left. Se fallisce (exit≠0), esegue right.
 * Altrimenti ritorna l'exit status di left.
 * 
 * Return: Exit status dell'ultimo comando eseguito
 */
int	execute_or(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	if (exit_status != 0)
		exit_status = execute(node->right, ctx);
	return (exit_status);
}


