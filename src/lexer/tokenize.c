/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/30 21:24:44 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                        CHARACTER TYPE CHECKERS                            */
/* ========================================================================== */

/**
 * char_is_space - Verifica se un carattere è uno spazio bianco
 * @c: Il carattere da verificare
 * 
 * Return: 1 se è spazio/tab/newline, 0 altrimenti
 */
static int	char_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

/**
 * char_is_operator - Verifica se un carattere è un operatore
 * @c: Il carattere da verificare
 * 
 * Return: 1 se è un carattere operatore, 0 altrimenti
 */
static int	char_is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')' || c == ';');
}

/* ========================================================================== */
/*                         TOKEN READING FUNCTION                            */
/* ========================================================================== */

/**
 * get_next_token - Estrae il prossimo token dalla stringa di input
 * @s: La stringa di input
 * @i: Puntatore alla posizione corrente nella stringa
 * 
 * Questa funzione:
 * 1. Salta gli spazi bianchi
 * 2. Determina il tipo di token (operatore o parola/quote)
 * 3. Chiama la funzione appropriata per leggere il token
 * 4. Crea e restituisce il token con il tipo corretto
 * 
 * Le quote sono ora gestite da read_word per supportare casi come:
 * - "hello"world (parola singola)
 * - hello"world"test (parola singola)
 * 
 * Return: Il token letto, o NULL se non ci sono più token
 */
static t_token	*get_next_token(const char *s, int *i)
{
	t_token_type	type;
	char			*value;
	t_token			*tok;

	while (s[*i] && char_is_space(s[*i]))
		(*i)++;
	if (!s[*i])
		return (NULL);
	if (char_is_operator(s[*i]))
	{
		value = read_operator(s, i, &type);
		if (!value)
			return (NULL);
		tok = token_create(type, value);
		free(value);
		return (tok);
	}
	value = read_word(s, i);
	if (!value)
		return (NULL);
	tok = token_create(TOKEN_WORD, value);
	free(value);
	return (tok);
}

/* ========================================================================== */
/*                       MAIN TOKENIZATION FUNCTION                          */
/* ========================================================================== */

/**
 * tokenize - Converte una stringa di input in un array di token
 * @input: La stringa di input da tokenizzare
 * 
 * Questa è la funzione principale del lexer. Processa l'intera stringa
 * di input carattere per carattere, estraendo i token e costruendo
 * un array dinamico che li contiene.
 * 
 * Il processo:
 * 1. Alloca un array iniziale vuoto
 * 2. Estrae token uno alla volta con get_next_token()
 * 3. Aggiunge ogni token all'array con token_array_add()
 * 4. Continua finché non ci sono più caratteri
 * 
 * Return: Array di token NULL-terminated, o NULL in caso di errore
 */
t_token	**tokenize(const char *input)
{
	t_token	**tokens;
	t_token	*tok;
	int		i;

	tokens = malloc(sizeof(t_token *));
	if (!tokens)
		return (NULL);
	tokens[0] = NULL;
	i = 0;
	while (input[i])
	{
		tok = get_next_token(input, &i);
		if (!tok)
			break ;
		if (!token_array_add(&tokens, tok))
		{
			token_array_free(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
