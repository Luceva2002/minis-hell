/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	apply_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (perror(filename), 0);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2"), close(fd), 0);
	close(fd);
	return (1);
}

int	apply_output_redirect(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (perror(filename), 0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2"), close(fd), 0);
	close(fd);
	return (1);
}

static int	apply_single_redirect(t_redirect *redir)
{
	if (redir->status == 0)
		return (apply_input_redirect(redir->filename));
	if (redir->status == 1)
		return (apply_output_redirect(redir->filename, 0));
	if (redir->status == 2)
		return (apply_output_redirect(redir->filename, 1));
	if (redir->status == 3)
		return (apply_heredoc(redir->filename));
	return (1);
}

int	apply_redirections(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return (1);
	i = 0;
	while (redirects[i])
	{
		if (!apply_single_redirect(redirects[i]))
			return (0);
		i++;
	}
	return (1);
}
