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

/* ========================================================================== */
/*                          INPUT REDIRECTION                                */
/* ========================================================================== */

/**
 * apply_input_redirect - Applica redirect input: < file
 * @filename: Nome del file da cui leggere l'input
 * 
 * Apre il file in lettura e reindirizza stdin su di esso.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore
 */
int	apply_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/* ========================================================================== */
/*                         OUTPUT REDIRECTION                                */
/* ========================================================================== */

/**
 * apply_output_redirect - Applica redirect output: > file o >> file
 * @filename: Nome del file su cui scrivere
 * @append: 1 per append (>>), 0 per truncate (>)
 * 
 * Apre il file in scrittura e reindirizza stdout su di esso.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore
 */
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
	{
		perror(filename);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/* ========================================================================== */
/*                            HEREDOC                                        */
/* ========================================================================== */

/**
 * apply_heredoc - Applica heredoc: << DELIMITER
 * @delimiter: Stringa delimiter che termina l'input
 * 
 * Legge linee dall'utente fino a quando non viene digitato il delimiter.
 * Le linee vengono scritte in una pipe e stdin viene reindirizzato
 * a leggere dalla pipe.
 * Gestisce Ctrl+C per interrompere l'input del heredoc.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore/interruzione
 */
int	apply_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (0);
	}
	g_signal = 0;
	setup_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			free(line);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			setup_signals();
			return (0);
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
	close(pipe_fd[1]);
	setup_signals();
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		return (perror("dup2"), close(pipe_fd[0]), 0);
	close(pipe_fd[0]);
	return (1);
}

/* ========================================================================== */
/*                      APPLY ALL REDIRECTIONS                               */
/* ========================================================================== */

/**
 * apply_redirections - Applica tutte le redirections di un comando
 * @redirects: Array di redirections da applicare
 * 
 * Itera su tutte le redirections e le applica nell'ordine.
 * Le redirections successive sovrascrivono le precedenti (bash behavior).
 * 
 * Stati delle redirections:
 * - 0: input redirect (<)
 * - 1: output redirect (>)
 * - 2: append (>>)
 * - 3: heredoc (<<)
 * 
 * Return: 1 se tutte hanno successo, 0 se una fallisce
 */
int	apply_redirections(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return (1);
	i = 0;
	while (redirects[i])
	{
		if (redirects[i]->status == 0)
		{
			if (!apply_input_redirect(redirects[i]->filename))
				return (0);
		}
		else if (redirects[i]->status == 1)
		{
			if (!apply_output_redirect(redirects[i]->filename, 0))
				return (0);
		}
		else if (redirects[i]->status == 2)
		{
			if (!apply_output_redirect(redirects[i]->filename, 1))
				return (0);
		}
		else if (redirects[i]->status == 3)
		{
			if (!apply_heredoc(redirects[i]->filename))
				return (0);
		}
		i++;
	}
	return (1);
}


