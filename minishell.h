#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include "libft.h"
#include "ft_printf.h"
#include "get_next_line_bonus.h"


typedef enum e_toktype {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC
}   t_toktype;

typedef struct s_token
{
    char *content;
    t_type type;
    int pos;
    struct s_token *next;
} t_token;

typedef struct s_quote
{
	char	*content;
	int		pos;
	struct s_quote	*next;
}	t_quote;

static bool	is_quote(char c);
void	copy_in_quotes(char	*content, t_quote **lst);
t_quote	*add_quote_node(t_quote **lst, char *content, int pos);
char	*remove_quotes(char *str);
t_token	*handle_split(char	*line);
static	t_token *new_token(char *content, t_toktype type, int pos);
t_token *merge_matrice_and_quotes(char **matrice, t_quote *quotes);
t_command   *new_command(void);
void    add_arg_to_command(t_command *cmd, const char *arg);
void	set_infile(t_command *cmd, char *file);
void	set_outfile(t_command *cmd, char *file, int append);
void	append_command(t_command **head, t_command *new);
t_command   *parse_tokens(t_token   *tokens);

int main(int ac, char **av);
#endif
