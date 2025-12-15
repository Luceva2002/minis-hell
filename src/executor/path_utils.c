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
#include <sys/stat.h>

int	handle_path_error(char *cmd)
{
	struct stat	st;

	if (stat(cmd, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": is a directory\n", 2);
			return (126);
		}
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

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

static int	is_valid_executable(char *path)
{
	struct stat	st;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	return (1);
}

char	*find_executable_in_path(char *cmd_name, t_env *env)
{
	char	*path_env;
	char	**dirs;

	if (ft_strchr(cmd_name, '/'))
	{
		if (is_valid_executable(cmd_name))
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
