/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:45:40 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                         TOKEN CREATION FUNCTIONS                          */
/* ========================================================================== */

/**
 * token_create - Crea un nuovo token allocato in memoria
 * @type: Il tipo del token (TOKEN_WORD, TOKEN_PIPE, ecc.)
 * @value: La stringa contenuta nel token
 * 
 * Alloca e inizializza un nuovo token con il tipo e valore specificati.
 * Tutti i campi vengono inizializzati (value viene duplicato).
 * 
 * Return: Il token creato, o NULL in caso di errore di allocazione
 */
t_token	*token_create(t_token_type type, const char *value)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = ft_strdup(value);
	tok->content = NULL;
	tok->pos = 0;
	tok->next = NULL;
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	return (tok);
}

/* ========================================================================== */
/*                      TOKEN ARRAY MANAGEMENT FUNCTIONS                     */
/* ========================================================================== */

/**
 * token_array_count - Conta il numero di token in un array
 * @tokens: Array di puntatori a token (NULL-terminated)
 * 
 * Return: Il numero di token nell'array
 */
static int	token_array_count(t_token **tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens[count])
		count++;
	return (count);
}

/**
 * token_array_add - Aggiunge un token a un array dinamico
 * @tokens: Puntatore all'array di token da espandere
 * @tok: Il token da aggiungere all'array
 * 
 * Espande l'array di token aggiungendo un nuovo elemento in coda.
 * L'array viene riallocato per contenere il nuovo token.
 * Il vecchio array viene liberato e sostituito con quello nuovo.
 * 
 * Return: 1 in caso di successo, 0 in caso di errore di allocazione
 */
int	token_array_add(t_token ***tokens, t_token *tok)
{
	t_token	**new_arr;
	int		count;
	int		i;

	count = token_array_count(*tokens);
	new_arr = malloc(sizeof(t_token *) * (count + 2));
	if (!new_arr)
		return (0);
	i = 0;
	while (i < count)
	{
		new_arr[i] = (*tokens)[i];
		i++;
	}
	new_arr[count] = tok;
	new_arr[count + 1] = NULL;
	if (*tokens)
		free(*tokens);
	*tokens = new_arr;
	return (1);
}

/* ========================================================================== */
/*                         TOKEN CLEANUP FUNCTIONS                           */
/* ========================================================================== */

/**
 * token_array_free - Libera un array di token e tutti i suoi elementi
 * @tokens: Array di token da liberare (NULL-terminated)
 * 
 * Libera ricorsivamente tutti i token nell'array e l'array stesso.
 * Ogni token viene completamente deallocato (incluse le stringhe interne).
 */
void	token_array_free(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->value)
			free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
