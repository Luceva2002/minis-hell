/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_interrupted(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	setup_signals();
	return (0);
}

static int	heredoc_read_lines(int pipe_fd[2], char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			free(line);
			return (heredoc_interrupted(pipe_fd));
		}
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	return (1);
}

int	apply_heredoc(char *delimiter)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 0);
	g_signal = 0;
	setup_signals_heredoc();
	if (!heredoc_read_lines(pipe_fd, delimiter))
		return (0);
	close(pipe_fd[1]);
	setup_signals();
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (perror("dup2"), close(pipe_fd[0]), 0);
	close(pipe_fd[0]);
	return (1);
}
