/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_dirs_array(char **dirs)
{
	int	i;

	if (!dirs)
		return ;
	i = 0;
	while (dirs[i])
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
}

static char	*search_in_dirs(char **dirs, char *cmd_name)
{
	char	*full_path;
	char	*tmp;
	int		i;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_dirs_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_dirs_array(dirs);
	return (NULL);
}

static int	is_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

char	*find_executable_in_path(char *cmd_name, t_env *env)
{
	char	*path_env;
	char	**dirs;

	if (is_path(cmd_name))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	path_env = env_get("PATH", env);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	return (search_in_dirs(dirs, cmd_name));
}
