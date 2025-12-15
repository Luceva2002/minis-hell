/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/02 21:41:48 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

volatile sig_atomic_t	g_signal = 0;

void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * handle_sigint_heredoc - Gestore SIGINT durante heredoc
 * @sig: Numero del segnale
 * 
 * Durante l'inserimento di heredoc, Ctrl+C deve interrompere
 * l'input e tornare al prompt principale.
 */
static void	handle_sigint_heredoc(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

/* ========================================================================== */
/*                         SIGNAL SETUP                                      */
/* ========================================================================== */

/**
 * setup_signals - Configura i gestori di segnali per la shell
 * 
 * Configura:
 * - SIGINT (Ctrl+C): mostra nuovo prompt senza terminare
 * - SIGQUIT (Ctrl+\): ignorato (SIG_IGN) in modalità interattiva
 * 
 * Questi comportamenti sono standard per le shell interattive.
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/**
 * setup_signals_child - Configura segnali per processo figlio
 * 
 * Nel processo figlio, i segnali devono avere il comportamento di default:
 * - SIGINT termina il processo
 * - SIGQUIT termina il processo con core dump
 */
void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * setup_signals_heredoc - Configura segnali durante heredoc
 * 
 * Durante l'input del heredoc:
 * - SIGINT interrompe l'input
 * - SIGQUIT ignorato
 */
void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
