#include "minishell.h"

t_token	*handle_split(char	*line)
{
	t_quote *quotes = NULL;
	char *line_copy;
	char **matrice;
	t_token	*tokens = NULL;

	if(!check_quote(line))
	{
		printf("syntax error: unclosed quote\n");
		return (NULL);
	}
	copy_in_quotes(line, &quotes);
	line_copy = remove_quotes(line);
	matrice = ft_split(line_copy, ' ');
	free(line_copy);
	tokens = merge_matrice_and_quotes(matrice, quotes);
	return (tokens);
}

static	t_token *new_token(char *content, t_toktype type, int pos)
{
	t_token *node = malloc(sizeof(t_token));
	if (!node)
		return NULL;
	node->content = content;
	node->type = type;
	node->pos = pos;
	node->next = NULL;
	return (node);
}

t_token *merge_matrice_and_quotes(char **matrice, t_quote *quotes)
{
	t_token *head = NULL;
	t_token *tail = NULL;
	int	i = 0;
	int	word_pos = 1;

	while (matrice[i] || quotes)
	{
		if (quotes && quotes->pos == word_pos)
		{
			t_token *node = new_token(quotes->content, T_WORD, quotes->pos);
			if (!head)
				head = node;
			else
				tail->next = node;
			tail = node;
			quotes = quotes->next;
		}
		else if (matrice[i])
		{
			t_toktype type = T_WORD;
			if (strcmp(matrice[i], "|") == 0)
				type = T_PIPE;
			else if (strcmp(matrice[i], "<") == 0)
				type = T_REDIR_IN;
			else if (strcmp(matrice[i], ">") == 0)
				type = T_REDIR_OUT;
			else if (strcmp(matrice[i], ">>") == 0)
				type = T_APPEND;
			else if (strcmp(matrice[i], "<<") == 0)
				type = T_HEREDOC;
			t_token *node = new_token(matrice[i], type, word_pos);
			if (!head)
				head = node;
			else
				tail->next = node;
			tail = node;
			i++;
		}
		word_pos++;
	}
	return (head);
}
