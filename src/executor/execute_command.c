/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/08 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

/* ========================================================================== */
/*                         PATH RESOLUTION                                   */
/* ========================================================================== */

/**
 * is_path - Controlla se il comando contiene un path (relativo o assoluto)
 * @cmd: Nome del comando
 * 
 * Return: 1 se contiene '/', 0 altrimenti
 */
static int	is_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

/**
 * free_dirs_array - Libera un array di directory
 * @dirs: Array da liberare
 */
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

/**
 * find_executable_in_path - Cerca l'eseguibile nelle directory di PATH
 * @cmd_name: Nome del comando da cercare
 * @env: Lista delle variabili d'ambiente
 * 
 * Estrae la variabile PATH, la splitta per ':' e cerca il comando
 * in ogni directory. Controlla l'esistenza e l'eseguibilità con access().
 * 
 * Return: Path completo dell'eseguibile se trovato, NULL altrimenti
 */
char	*find_executable_in_path(char *cmd_name, t_env *env)
{
	char	*path_env;
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	path_env = env_get("PATH", env);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
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

/**
 * resolve_command_path - Risolve il path del comando
 * @cmd: Nome o path del comando
 * @env: Lista delle variabili d'ambiente
 * 
 * Se il comando contiene '/', usa il path direttamente.
 * Altrimenti cerca nel PATH.
 * 
 * Return: Path del comando o NULL se non trovato
 */
static char	*resolve_command_path(char *cmd, t_env *env)
{
	if (is_path(cmd))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (find_executable_in_path(cmd, env));
}

/* ========================================================================== */
/*                      EXTERNAL COMMAND EXECUTION                           */
/* ========================================================================== */

/**
 * execute_in_child - Esegue il comando nel processo figlio
 * @cmd: Struttura comando con args e redirections
 * @executable_path: Path completo dell'eseguibile
 * @ctx: Contesto della shell
 * 
 * Questa funzione viene chiamata nel processo figlio dopo fork().
 * Applica le redirections, converte l'environment in array e chiama execve.
 * Se execve fallisce, stampa l'errore ed esce con il codice appropriato.
 */
static void	execute_in_child(t_command *cmd, char *executable_path,
				t_shell_context *ctx)
{
	char	**envp;

	setup_signals_child();
	if (cmd->redirect)
	{
		if (!apply_redirections(cmd->redirect))
			exit(1);
	}
	envp = env_to_array(ctx->env);
	if (!envp)
	{
		ft_putstr_fd("minishell: malloc error\n", 2);
		exit(1);
	}
	execve(executable_path, cmd->args, envp);
	perror("minishell");
	exit(127);
}

/**
 * execute_external_command - Fork + execve per comando esterno
 * @cmd: Struttura comando con args e redirections
 * @ctx: Contesto della shell
 * 
 * Algoritmo:
 * 1. Risolve il path del comando (cerca in PATH o usa path diretto)
 * 2. Fork processo figlio
 * 3. Figlio: applica redirections ed esegue execve
 * 4. Padre: aspetta il figlio e ritorna l'exit status
 * 
 * Return: Exit status del comando
 */
int	execute_external_command(t_command *cmd, t_shell_context *ctx)
{
	char	*executable_path;
	pid_t	pid;
	int		status;

	executable_path = resolve_command_path(cmd->args[0], ctx->env);
	if (!executable_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (free(executable_path), 1);
	}
	if (pid == 0)
		execute_in_child(cmd, executable_path, ctx);
	free(executable_path);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (write(1, "\n", 1), 130);
		if (WTERMSIG(status) == SIGQUIT)
			return (ft_putendl_fd("Quit: 3", 2), 131);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}


