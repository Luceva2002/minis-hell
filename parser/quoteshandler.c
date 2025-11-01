#include "minishell.h"


static bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

void	copy_in_quotes(char	*content, t_quote **lst)
{
	int	i;
	int	pos;
	int	begin;
	int	end;

	i = 0;
	pos = 1;
	while(content[i])
	{
		while(!is_quote(content[i]))
			i++;
		begin = i+1;
		i++;
		while(!is_quote(content[i]))
			i++;
		end = i;
		add_quote_node(lst,ft_strndup(content + begin, end - begin), pos);
		pos ++;
		i++;
	}
}

t_quote	*add_quote_node(t_quote **lst, char *content, int pos)
{
	t_quote	*new = malloc(sizeof(t_quote));
	t_quote	*tmp;

	if (!new)
		return (NULL);
	new->content = content;
	new->pos = pos;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (*lst);
}

char	*remove_quotes(char *str)
{
	int		i = 0;
	int		j = 0;
	char	*res;
	int		len = 0;

	while (str[i])
	{
		if (is_quote(str[i]))
		{
			i++;
			while (str[i] && !is_quote(str[i]))
				i++;
			if (str[i])
				i++;
		}
		else
			i++;
		len++;
	}
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (is_quote(*str))
		{
			str++;
			while (*str && !is_quote(*str))
				res[j++] = *str++;
			if (*str)
				str++;
		}
		else
			res[j++] = *str++;
	}
	res[j] = '\0';
	return (res);
}
