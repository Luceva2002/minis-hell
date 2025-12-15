/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:22:19 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <unistd.h>

void	print_banner(void);

static int	validate_and_parse(t_token **tokens, t_ast_node **ast,
	t_shell_context *ctx)
{
	int	syntax_status;

	syntax_status = syntax_validate(tokens);
	if (syntax_status != SYNTAX_OK)
	{
		syntax_print_error(syntax_status);
		token_array_free(tokens);
		return (0);
	}
	*ast = parse(tokens, ctx);
	if (!*ast)
	{
		token_array_free(tokens);
		return (0);
	}
	return (1);
}

static void	process_input(char *line, t_shell_context *ctx)
{
	t_token		**tokens;
	t_ast_node	*ast;

	tokens = tokenize(line);
	if (!tokens)
	{
		ctx->last_exit_code = 1;
		return ;
	}
	if (!validate_and_parse(tokens, &ast, ctx))
	{
		ctx->last_exit_code = 2;
		return ;
	}
	ctx->last_exit_code = execute(ast, ctx);
	ast_node_free(ast);
	token_array_free(tokens);
}

static void	shell_loop(t_shell_context *ctx)
{
	char	*line;

	while (ctx->running)
	{
		g_signal = 0;
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (g_signal == SIGINT)
		{
			ctx->last_exit_code = 130;
			g_signal = 0;
		}
		else if (*line)
		{
			add_history(line);
			process_input(line, ctx);
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_context	ctx;

	(void)argc;
	(void)argv;
	setup_signals();
	ctx.env = env_init(envp);
	if (!ctx.env)
	{
		ft_putstr_fd("minishell: failed to initialize env\n", 2);
		return (1);
	}
	ctx.last_exit_code = 0;
	ctx.running = 1;
	if (isatty(STDIN_FILENO))
		print_banner();
	shell_loop(&ctx);
	env_free(ctx.env);
	return (ctx.last_exit_code);
}
