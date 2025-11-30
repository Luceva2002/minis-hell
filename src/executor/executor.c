/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/30 21:24:44 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

/* ========================================================================== */
/*                         MAIN EXECUTOR FUNCTION                            */
/* ========================================================================== */

/**
 * execute_subshell - Esegue una subshell (comandi tra parentesi)
 * @node: Nodo AST di tipo NODE_SUBSHELL
 * @ctx: Contesto della shell
 * 
 * Crea un processo figlio per eseguire i comandi della subshell.
 * La subshell eredita l'environment ma le modifiche non persistono.
 * 
 * Return: Exit status dell'ultimo comando della subshell
 */
static int	execute_subshell(t_ast_node *node, t_shell_context *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		setup_signals_child();
		exit(execute(node->left, ctx));
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (write(1, "\n", 1), 130);
		if (WTERMSIG(status) == SIGQUIT)
			return (ft_putendl_fd("Quit: 3", 2), 131);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * execute_seq - Esegue una sequenza di comandi (;)
 * @node: Nodo AST di tipo NODE_SEQ
 * @ctx: Contesto della shell
 * 
 * Esegue il comando sinistro, poi quello destro.
 * Aggiorna last_exit_code dopo ogni comando per $? corretto.
 * 
 * Return: Exit status dell'ultimo comando
 */
static int	execute_seq(t_ast_node *node, t_shell_context *ctx)
{
	int	exit_status;

	exit_status = execute(node->left, ctx);
	ctx->last_exit_code = exit_status;
	exit_status = execute(node->right, ctx);
	return (exit_status);
}

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
 * - NODE_SEQ: sequenza (;)
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
		exit_status = execute_subshell(ast, ctx);
	else if (ast->type == NODE_SEQ)
		exit_status = execute_seq(ast, ctx);
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
 * expand_command_args - Espande le variabili negli argomenti del comando
 * @cmd: Comando con argomenti da espandere
 * @ctx: Contesto della shell
 * 
 * Espande $VAR e $? in tutti gli argomenti e filename delle redirezioni.
 * L'espansione avviene al momento dell'esecuzione, non del parsing.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore
 */
static int	expand_command_args(t_command *cmd, t_shell_context *ctx)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->args)
		return (1);
	i = 0;
	while (cmd->args[i])
	{
		expanded = process_token_value(cmd->args[i], ctx);
		if (!expanded)
			return (0);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
		i++;
	}
	if (cmd->redirect)
	{
		i = 0;
		while (cmd->redirect[i])
		{
			if (cmd->redirect[i]->status != 3)
			{
				expanded = process_token_value(cmd->redirect[i]->filename, ctx);
				if (!expanded)
					return (0);
				free(cmd->redirect[i]->filename);
				cmd->redirect[i]->filename = expanded;
			}
			i++;
		}
	}
	return (1);
}

/**
 * execute_simple_command - Esegue un singolo comando
 * @node: Nodo AST di tipo NODE_CMD
 * @ctx: Contesto della shell
 * 
 * Espande le variabili negli argomenti, poi controlla se il comando 
 * è un built-in ed eseguilo direttamente, altrimenti delega a 
 * execute_external_command per fork+execve.
 * 
 * Return: Exit status del comando
 */
int	execute_simple_command(t_ast_node *node, t_shell_context *ctx)
{
	if (!node || !node->cmd || !node->cmd->args || !node->cmd->args[0])
		return (0);
	if (!expand_command_args(node->cmd, ctx))
		return (1);
	if (!node->cmd->args[0] || !node->cmd->args[0][0])
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


