#include "minishell.h"

//to see

//command creation
t_command   *new_command(void)
{
    t_command   *cmd;

    cmd = malloc(sizeof(t_command));
    if(!cmd)
        return (NULL);
    cmd -> args = NULL;
    cmd -> infile = NULL;
    cmd -> outfile = NULL;
    cmd -> append = 0;
    cmd -> next = NULL;
    return cmd;
}

//add arg
void    add_arg_to_command(t_command *cmd, const char *arg)
{
    int i;
    char **new_args;

    i = 0;
    if (!cmd)
        return ;
    while (cmd -> args && cmd -> args[i])
        i++;
    new_args = malloc(sizeof(char *) + (i + 2));
    if (!new_args)
        return;
    i = -1;
    while (cmd -> args && cmd -> args[++i])
        new_args[i] = cmd -> args[i];
    new_args[i] = strdup(arg);
    new_args[i + 1] = NULL;
    if (cmd -> args)
        free(cmd ->args);
    cmd -> args = new_args;
}

// redirections
void	set_infile(t_command *cmd, char *file)
{
	if (!cmd || !file)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = strdup(file);
}

void	set_outfile(t_command *cmd, char *file, int append)
{
	if (!cmd || !file)
		return ;
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = strdup(file);
	cmd->append = append;
}

//append command
void	append_command(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
