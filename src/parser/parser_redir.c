/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:40:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                    REDIRECTION MANAGEMENT                                 */
/* ========================================================================== */

/**
 * count_redirects - Conta le redirezioni in un array
 * @redirs: Array di redirezioni (NULL-terminated)
 * 
 * Return: Numero di redirezioni
 */
static int	count_redirects(t_redirect **redirs)
{
	int	i;

	i = 0;
	while (redirs && redirs[i])
		i++;
	return (i);
}

/**
 * cmd_add_redirect - Aggiunge una redirezione a un comando
 * @cmd: Il comando a cui aggiungere la redirezione
 * @redir: La redirezione da aggiungere
 * 
 * Espande l'array di redirezioni per includere la nuova redirezione.
 * La redirezione viene aggiunta in coda all'array.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore
 */
int	cmd_add_redirect(t_command *cmd, t_redirect *redir)
{
	int			i;
	int			count;
	t_redirect	**new_redir;

	if (!cmd || !redir)
		return (0);
	count = count_redirects(cmd->redirect);
	new_redir = malloc(sizeof(t_redirect *) * (count + 2));
	if (!new_redir)
		return (0);
	i = -1;
	while (++i < count)
		new_redir[i] = cmd->redirect[i];
	new_redir[i] = redir;
	new_redir[i + 1] = NULL;
	if (cmd->redirect)
		free(cmd->redirect);
	cmd->redirect = new_redir;
	return (1);
}
