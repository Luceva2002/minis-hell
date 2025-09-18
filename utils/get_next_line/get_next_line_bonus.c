/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:08:23 by luevange          #+#    #+#             */
/*   Updated: 2025/03/07 16:22:42 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	update_line(char *stash)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	while (stash[i] != '\0')
	{
		stash[j] = stash[i];
		j++;
		i++;
	}
	while (stash[j])
	{
		stash[j] = '\0';
		j++;
	}
}

static char	*extract_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	ft_strlcpy(line, stash, i + 1);
	return (line);
}

static char	*process_stash(char **line, char *stash)
{
	char	*str;

	if (ft_strchar(stash, '\n'))
	{
		str = extract_line(stash);
		*line = ft_strjoin(*line, str);
		free(str);
		update_line(stash);
		return (*line);
	}
	*line = ft_strjoin(*line, stash);
	update_line(stash);
	return (NULL);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	stash[1024][BUFFER_SIZE + 1];
	int			bytes_read;
	char		*result;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		result = process_stash(&line, stash[fd]);
		if (result)
			return (result);
		bytes_read = read(fd, stash[fd], BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			if (bytes_read == 0 && ft_strlen(line) > 0)
				return (line);
			free(line);
			return (NULL);
		}
	}
}

/* int	main(void)
{
	int fd;
	int i = 15;
	char *line;
	fd = open("test.txt", O_RDONLY);
	while (i)
	{
		line = get_next_line(fd);
		printf("%s", line);
		free(line);
		i--;
	}
} */