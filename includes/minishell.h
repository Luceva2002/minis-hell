/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:19 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:20:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                              SYSTEM HEADERS                               */
/* ========================================================================== */

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>
# include "../utils/includes/libft.h"
# include "../utils/includes/ft_printf.h"

/* ========================================================================== */
/*                           SYNTAX ERROR CODES                              */
/* ========================================================================== */

# define SYNTAX_OK 0
# define SYNTAX_UNCLOSED 1
# define SYNTAX_OPERATOR 2
# define SYNTAX_PARENS 3

/* ========================================================================== */
/*                              TOKEN DEFINITIONS                            */
/* ========================================================================== */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			*content;
	int				pos;
	struct s_token	*next;
}	t_token;

/* ========================================================================== */
/*                            AST NODE DEFINITIONS                           */
/* ========================================================================== */

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_redirect
{
	int		status;
	char	*filename;
}	t_redirect;

typedef struct s_command
{
	char		*comm;
	char		**args;
	t_redirect	**redirect;
}	t_command;

typedef struct s_ast_node
{
	t_node_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command			*cmd;
}	t_ast_node;

/* ========================================================================== */
/*                           LEXER FUNCTIONS                                 */
/* ========================================================================== */

/**
 * TOKENIZZAZIONE
 * tokenize         - Converte una stringa di input in array di token
 * read_quoted      - Legge una stringa tra quote
 * read_operator    - Legge un operatore (<, >, |, &&, ||, ecc.)
 * read_word        - Legge una parola (argomento o comando)
 */
t_token		**tokenize(const char *input);
char		*read_quoted(const char *s, int *i);
char		*read_operator(const char *s, int *i, t_token_type *type);
char		*read_word(const char *s, int *i);

/**
 * GESTIONE TOKEN
 * token_create     - Crea un nuovo token
 * token_array_add  - Aggiunge un token a un array
 * token_array_free - Libera un array di token
 */
t_token		*token_create(t_token_type type, const char *value);
int			token_array_add(t_token ***tokens, t_token *tok);
void		token_array_free(t_token **tokens);

/* ========================================================================== */
/*                         VALIDATION FUNCTIONS                              */
/* ========================================================================== */

/**
 * VALIDAZIONE SINTASSI
 * syntax_validate          - Valida la sintassi generale
 * syntax_check_operators   - Verifica sequenza corretta degli operatori
 * syntax_check_parens      - Verifica bilanciamento parentesi
 * syntax_print_error       - Stampa messaggio di errore
 */
int			syntax_validate(t_token **tokens);
int			syntax_check_operators(t_token **tokens);
int			syntax_check_parens(t_token **tokens);
void		syntax_print_error(int error_code);

/* ========================================================================== */
/*                           PARSER FUNCTIONS                                */
/* ========================================================================== */

/**
 * PARSING PRINCIPALE
 * parse                    - Punto di ingresso del parser, genera l'AST
 * parse_or                 - Parsing operatore OR (||)
 * parse_and                - Parsing operatore AND (&&)
 * parse_pipe               - Parsing operatore PIPE (|)
 * parse_primary            - Parsing espressioni primarie e subshell
 * parse_redir_and_cmd      - Parsing comando con redirect
 */
t_ast_node	*parse(t_token **tokens);
t_ast_node	*parse_or(t_token **tokens, int *pos);
t_ast_node	*parse_and(t_token **tokens, int *pos);
t_ast_node	*parse_pipe(t_token **tokens, int *pos);
t_ast_node	*parse_primary(t_token **tokens, int *pos);
t_ast_node	*parse_redir_and_cmd(t_token **tokens, int *pos);

/**
 * CREAZIONE NODI AST
 * ast_node_create          - Crea un nuovo nodo AST
 * ast_node_free            - Libera un nodo AST e i suoi figli
 * cmd_node_create          - Crea un nuovo nodo comando
 * cmd_add_argument         - Aggiunge un argomento a un comando
 * cmd_add_redirect         - Aggiunge una redirezione a un comando
 */
t_ast_node	*ast_node_create(t_node_type type);
void		ast_node_free(t_ast_node *node);
t_command	*cmd_node_create(void);
int			cmd_add_argument(t_command *cmd, char *arg);
int			cmd_add_redirect(t_command *cmd, t_redirect *redir);

#endif
