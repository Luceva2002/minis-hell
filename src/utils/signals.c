/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/15 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <signal.h>

/* ========================================================================== */
/*                         SIGNAL HANDLER                                    */
/* ========================================================================== */

/**
 * handle_sigint - Gestore per SIGINT (Ctrl+C)
 * @sig: Numero del segnale (non usato)
 * 
 * Quando l'utente preme Ctrl+C nella shell:
 * 1. Va a capo
 * 2. Mostra un nuovo prompt
 * 3. Non termina il programma
 * 
 * Usa rl_on_new_line() e rl_redisplay() per gestire readline.
 */
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
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
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

