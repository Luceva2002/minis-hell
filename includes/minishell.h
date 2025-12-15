/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:19 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 21:38:30 by luevange         ###   ########.fr       */
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
# include <signal.h>
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
	TOKEN_RPAREN,
	TOKEN_SEMICOLON
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
	NODE_SUBSHELL,
	NODE_SEQ
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
/*                        ENVIRONMENT DEFINITIONS                            */
/* ========================================================================== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell_context
{
	t_env	*env;
	int		last_exit_code;
	int		running;
}	t_shell_context;

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
t_ast_node	*parse(t_token **tokens, t_shell_context *ctx);
t_ast_node	*parse_or(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_and(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_pipe(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_primary(t_token **tokens, int *pos, t_shell_context *ctx);
t_ast_node	*parse_redir_and_cmd(t_token **tokens, int *pos,
				t_shell_context *ctx);

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

/* ========================================================================== */
/*                        ENVIRONMENT FUNCTIONS                              */
/* ========================================================================== */

/**
 * GESTIONE ENVIRONMENT
 * env_init         - Inizializza environment da envp
 * env_get          - Ottiene valore di una variabile
 * env_set          - Imposta/aggiorna una variabile
 * env_unset        - Rimuove una variabile
 * env_to_array     - Converte lista in array per execve
 * env_free         - Libera tutto l'environment
 */
t_env		*env_init(char **envp);
char		*env_get(char *key, t_env *env);
int			env_set(t_env **env, char *key, char *value);
int			env_unset(t_env **env, char *key);
char		**env_to_array(t_env *env);
void		env_free(t_env *env);

/* ========================================================================== */
/*                         EXECUTOR FUNCTIONS                                */
/* ========================================================================== */

/**
 * ESECUZIONE
 * execute                  - Punto di ingresso executor
 * execute_simple_command   - Esegue comando singolo
 * execute_pipe             - Esegue pipeline
 * execute_and              - Esegue operatore &&
 * execute_or               - Esegue operatore ||
 */
int			execute(t_ast_node *ast, t_shell_context *ctx);
int			execute_simple_command(t_ast_node *node, t_shell_context *ctx);
int			execute_pipe(t_ast_node *node, t_shell_context *ctx);
int			execute_and(t_ast_node *node, t_shell_context *ctx);
int			execute_or(t_ast_node *node, t_shell_context *ctx);

/**
 * COMANDI ESTERNI
 * execute_external_command - Fork + execve per comando esterno
 * find_executable_in_path  - Cerca comando nel PATH
 */
int			execute_external_command(t_command *cmd, t_shell_context *ctx);
char		*find_executable_in_path(char *cmd_name, t_env *env);

/**
 * REDIRECTIONS
 * apply_redirections       - Applica tutte le redirections
 * apply_input_redirect     - Redirect input <
 * apply_output_redirect    - Redirect output > o >>
 * apply_heredoc            - Heredoc <<
 */
int			apply_redirections(t_redirect **redirects);
int			apply_input_redirect(char *filename);
int			apply_output_redirect(char *filename, int append);
int			apply_heredoc(char *delimiter);

/**
 * BUILT-IN COMMANDS
 * is_builtin               - Controlla se è un built-in
 * execute_builtin          - Esegue built-in
 */
int			is_builtin(char *cmd);
int			execute_builtin(t_command *cmd, t_shell_context *ctx);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell_context *ctx);
int			builtin_pwd(void);
int			builtin_export(char **args, t_shell_context *ctx);
int			builtin_unset(char **args, t_shell_context *ctx);
int			builtin_env(t_shell_context *ctx);
int			builtin_exit(char **args, t_shell_context *ctx);

/* ========================================================================== */
/*                         EXPANSION & UTILS                                 */
/* ========================================================================== */

/**
 * EXPANSION FUNCTIONS
 * remove_quotes            - Rimuove quote da una stringa
 * expand_variables         - Espande $VAR e $?
 * process_token_value      - Processa token: rimuove quote ed espande
 */
char		*remove_quotes(char *str);
char		*expand_variables(char *str, t_shell_context *ctx, int in_quotes);
char		*process_token_value(char *value, t_shell_context *ctx);

/* ========================================================================== */
/*                         SIGNAL HANDLING                                   */
/* ========================================================================== */

/**
 * GLOBAL VARIABLE
 * g_signal - Unica variabile globale consentita dal subject
 * Memorizza il numero del segnale ricevuto (usata per exit status)
 */
extern volatile sig_atomic_t	g_signal;

/**
 * SIGNAL MANAGEMENT
 * setup_signals            - Configura gestione segnali
 * setup_signals_child      - Segnali per processo figlio
 * setup_signals_heredoc    - Segnali per heredoc
 * handle_sigint            - Gestore per Ctrl+C
 */
void		setup_signals(void);
void		setup_signals_child(void);
void		setup_signals_heredoc(void);
void		handle_sigint(int sig);

#endif
