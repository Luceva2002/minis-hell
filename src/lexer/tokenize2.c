/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:25:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                         STRING EXTRACTION HELPER                          */
/* ========================================================================== */

/**
 * extract_substring - Estrae una sottostringa e la copia in un nuovo buffer
 * @s: Stringa sorgente
 * @start: Indice di inizio dell'estrazione
 * @len: Lunghezza della sottostringa da estrarre
 * 
 * Return: La sottostringa allocata, o NULL in caso di errore
 */
static char	*extract_substring(const char *s, int start, int len)
{
	char	*result;
	int		i;

	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[len] = '\0';
	return (result);
}

/* ========================================================================== */
/*                         QUOTE READING FUNCTION                            */
/* ========================================================================== */

/**
 * read_quoted - Legge una stringa tra quote (singole o doppie)
 * @s: Stringa di input
 * @i: Puntatore alla posizione corrente (viene aggiornato)
 * 
 * Questa funzione legge una stringa racchiusa tra quote dello stesso tipo.
 * Include le quote nel risultato per preservare le informazioni sul tipo.
 * 
 * Processo:
 * 1. Salva il tipo di quote (apice singolo o doppio)
 * 2. Avanza oltre la quote di apertura
 * 3. Legge tutti i caratteri fino alla quote di chiusura
 * 4. Include anche la quote di chiusura
 * 5. Restituisce la stringa completa (con quote)
 * 
 * Return: La stringa estratta (con quote incluse)
 */
char	*read_quoted(const char *s, int *i)
{
	char	quote;
	int		start;
	int		j;
	char	*result;

	quote = s[*i];
	start = *i;
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
	result = malloc((*i - start) + 1);
	if (!result)
		return (NULL);
	j = 0;
	while (j < (*i - start))
	{
		result[j] = s[start + j];
		j++;
	}
	result[j] = '\0';
	return (result);
}

/* ========================================================================== */
/*                        OPERATOR TYPE DETECTION                            */
/* ========================================================================== */

/**
 * set_operator_type - Determina il tipo di operatore dalla stringa
 * @val: La stringa contenente l'operatore
 * @type: Puntatore al tipo da impostare
 * 
 * Analizza la stringa dell'operatore e imposta il tipo corrispondente.
 * Gestisce operatori a 1 e 2 caratteri (|, ||, <, <<, >, >>, &, &&, (, ))
 */
static void	set_operator_type(const char *val, t_token_type *type)
{
	if (val[0] == '|' && val[1] == '|')
		*type = TOKEN_OR;
	else if (val[0] == '|')
		*type = TOKEN_PIPE;
	else if (val[0] == '&' && val[1] == '&')
		*type = TOKEN_AND;
	else if (val[0] == '<' && val[1] == '<')
		*type = TOKEN_HEREDOC;
	else if (val[0] == '<')
		*type = TOKEN_REDIR_IN;
	else if (val[0] == '>' && val[1] == '>')
		*type = TOKEN_APPEND;
	else if (val[0] == '>')
		*type = TOKEN_REDIR_OUT;
	else if (val[0] == '(')
		*type = TOKEN_LPAREN;
	else if (val[0] == ')')
		*type = TOKEN_RPAREN;
}

/**
 * read_operator - Legge un operatore dalla stringa di input
 * @s: Stringa di input
 * @i: Puntatore alla posizione corrente (viene aggiornato)
 * @type: Puntatore al tipo di token (viene impostato)
 * 
 * Legge un operatore (1 o 2 caratteri) e determina il suo tipo.
 * 
 * Return: La stringa dell'operatore
 */
char	*read_operator(const char *s, int *i, t_token_type *type)
{
	int		start;
	char	*val;

	start = *i;
	if ((s[*i] == '|' && s[*i + 1] == '|')
		|| (s[*i] == '&' && s[*i + 1] == '&')
		|| (s[*i] == '<' && s[*i + 1] == '<')
		|| (s[*i] == '>' && s[*i + 1] == '>'))
		(*i) += 2;
	else
		(*i)++;
	val = extract_substring(s, start, *i - start);
	set_operator_type(val, type);
	return (val);
}

/**
 * read_word - Legge una parola (comando o argomento) dalla stringa
 * @s: Stringa di input
 * @i: Puntatore alla posizione corrente (viene aggiornato)
 * 
 * Legge tutti i caratteri consecutivi che non sono spazi, operatori o quote.
 * 
 * Return: La parola estratta
 */
char	*read_word(const char *s, int *i)
{
	int		start;

	start = *i;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t'
		&& s[*i] != '|' && s[*i] != '<' && s[*i] != '>'
		&& s[*i] != '&' && s[*i] != '(' && s[*i] != ')'
		&& s[*i] != '\'' && s[*i] != '"')
		(*i)++;
	return (extract_substring(s, start, *i - start));
}
