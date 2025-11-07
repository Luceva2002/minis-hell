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
 * 
 * Questa funzione gestisce tutto il flusso di elaborazione:
 * 1. LEXER: Tokenizza la linea di input
 * 2. VALIDATOR: Verifica la correttezza sintattica
 * 3. PARSER: Costruisce l'Abstract Syntax Tree (albero binario)
 * 4. (TODO) EXECUTOR: Esegue i comandi dall'AST
 * 
 * In caso di errore in qualsiasi fase, libera la memoria allocata
 * e stampa un messaggio appropriato.
 */
static void	process_line(char *line)
{
	t_token		**tokens;
	t_ast_node	*ast;
	int			syntax_status;

	tokens = tokenize(line);
	if (!tokens)
	{
		ft_printf("minishell: allocation error\n");
		return ;
	}
	syntax_status = syntax_validate(tokens);
	if (syntax_status != SYNTAX_OK)
	{
		syntax_print_error(syntax_status);
		token_array_free(tokens);
		return ;
	}
	ast = parse(tokens);
	if (!ast)
	{
		ft_printf("minishell: parse error\n");
		token_array_free(tokens);
		return ;
	}
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
 * @envp: Variabili d'ambiente (TODO: da utilizzare)
 * 
 * Loop principale del shell:
 * 1. Mostra il prompt con readline
 * 2. Legge l'input dell'utente
 * 3. Aggiunge all'history se non vuoto
 * 4. Processa la linea (lexer → validator → parser → executor)
 * 5. Ripete fino a EOF (Ctrl+D) o comando exit
 * 
 * Return: 0 in caso di uscita normale
 */
int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
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
			process_line(line);
		}
		free(line);
	}
	return (0);
}
