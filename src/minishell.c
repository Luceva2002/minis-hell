/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:19 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:35:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* ========================================================================== */
/*                         LINE PROCESSING FUNCTION                          */
/* ========================================================================== */

/**
 * process_line - Processa una linea di input completa
 * @line: La linea di input da processare
 * @ctx: Contesto della shell (environment, exit status, etc.)
 * 
 * Questa funzione gestisce tutto il flusso di elaborazione:
 * 1. LEXER: Tokenizza la linea di input
 * 2. VALIDATOR: Verifica la correttezza sintattica
 * 3. PARSER: Costruisce l'Abstract Syntax Tree (albero binario)
 * 4. EXECUTOR: Esegue i comandi dall'AST
 * 
 * In caso di errore in qualsiasi fase, libera la memoria allocata
 * e stampa un messaggio appropriato.
 */
static void	process_line(char *line, t_shell_context *ctx)
{
	t_token		**tokens;
	t_ast_node	*ast;
	int			syntax_status;
	int			exit_status;

	tokens = tokenize(line);
	if (!tokens)
	{
		ft_printf("minishell: allocation error\n");
		ctx->last_exit_code = 1;
		return ;
	}
	syntax_status = syntax_validate(tokens);
	if (syntax_status != SYNTAX_OK)
	{
		syntax_print_error(syntax_status);
		token_array_free(tokens);
		ctx->last_exit_code = 2;
		return ;
	}
	ast = parse(tokens, ctx);
	if (!ast)
	{
		ft_printf("minishell: parse error\n");
		token_array_free(tokens);
		ctx->last_exit_code = 2;
		return ;
	}
	exit_status = execute(ast, ctx);
	ctx->last_exit_code = exit_status;
	ast_node_free(ast);
	token_array_free(tokens);
}

/* ========================================================================== */
/*                            MAIN FUNCTION                                  */
/* ========================================================================== */

/**
 * main - Punto di ingresso del minishell
 * @argc: Numero di argomenti (non utilizzato)
 * @argv: Array di argomenti (non utilizzato)
 * @envp: Variabili d'ambiente del sistema
 * 
 * Loop principale del shell:
 * 1. Inizializza il contesto (environment, exit status, etc.)
 * 2. Mostra il prompt con readline
 * 3. Legge l'input dell'utente
 * 4. Aggiunge all'history se non vuoto
 * 5. Processa la linea (lexer → validator → parser → executor)
 * 6. Ripete fino a EOF (Ctrl+D) o comando exit
 * 
 * Return: Last exit code della shell
 */
int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_shell_context	ctx;

	(void)argc;
	(void)argv;
	setup_signals();
	ctx.env = env_init(envp);
	if (!ctx.env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	ctx.last_exit_code = 0;
	ctx.running = 1;
	while (ctx.running)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*line)
		{
			add_history(line);
			process_line(line, &ctx);
		}
		free(line);
	}
	env_free(ctx.env);
	return (ctx.last_exit_code);
}
