/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/15 00:00:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ========================================================================== */
/*                         QUOTE REMOVAL                                     */
/* ========================================================================== */

/**
 * remove_quotes - Rimuove le quote da una stringa
 * @str: Stringa che può contenere quote (es: "hello" o 'world')
 * 
 * Se la stringa inizia e finisce con lo stesso tipo di quote,
 * le rimuove e restituisce la stringa interna.
 * 
 * Return: Nuova stringa senza quote (allocata), o copia della stringa originale
 */
char	*remove_quotes(char *str)
{
	int		len;
	char	*result;

	if (!str || !*str)
		return (ft_strdup(""));
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

/* ========================================================================== */
/*                      VARIABLE EXPANSION HELPERS                           */
/* ========================================================================== */

/**
 * get_var_name_len - Calcola la lunghezza del nome di variabile dopo $
 * @str: Stringa che inizia dopo il $
 * 
 * Una variabile può contenere lettere, numeri e underscore.
 * Ma non può iniziare con un numero.
 * 
 * Return: Lunghezza del nome della variabile
 */
static int	get_var_name_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

/**
 * get_var_value - Ottiene il valore di una variabile dal contesto
 * @var_name: Nome della variabile (senza $)
 * @ctx: Contesto della shell
 * 
 * Gestisce casi speciali:
 * - $? ritorna l'exit code dell'ultimo comando
 * - Altre variabili vengono cercate nell'environment
 * 
 * Return: Valore della variabile (stringa allocata), o stringa vuota se non trovata
 */
static char	*get_var_value(char *var_name, t_shell_context *ctx)
{
	char	*value;

	if (!var_name || !*var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(ctx->last_exit_code));
	value = env_get(var_name, ctx->env);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

/* ========================================================================== */
/*                      STRING EXPANSION BUILDER                             */
/* ========================================================================== */

/**
 * append_string - Aggiunge una stringa a un buffer in crescita
 * @result: Puntatore al buffer risultato (può essere NULL)
 * @to_add: Stringa da aggiungere
 * 
 * Return: Nuovo buffer con la stringa aggiunta, NULL in caso di errore
 */
static char	*append_string(char *result, char *to_add)
{
	char	*new_result;

	if (!to_add)
		return (result);
	if (!result)
		return (ft_strdup(to_add));
	new_result = ft_strjoin(result, to_add);
	free(result);
	return (new_result);
}

/**
 * append_char - Aggiunge un carattere a un buffer in crescita
 * @result: Puntatore al buffer risultato (può essere NULL)
 * @c: Carattere da aggiungere
 * 
 * Return: Nuovo buffer con il carattere aggiunto, NULL in caso di errore
 */
static char	*append_char(char *result, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (append_string(result, tmp));
}

/* ========================================================================== */
/*                         MAIN EXPANSION FUNCTION                           */
/* ========================================================================== */

/**
 * expand_variables - Espande le variabili in una stringa
 * @str: Stringa che può contenere $VAR o $?
 * @ctx: Contesto della shell
 * @in_quotes: 1 se siamo dentro doppie quote, 0 altrimenti
 * 
 * Espande tutte le occorrenze di $VARIABILE con il loro valore.
 * Se in_quotes è 0 e la stringa ha singole quote, non espande.
 * 
 * Return: Nuova stringa con variabili espanse (allocata)
 */
char	*expand_variables(char *str, t_shell_context *ctx, int in_quotes)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		var_len;

	(void)in_quotes;
	if (!str)
		return (NULL);
	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
		{
			var_len = get_var_name_len(&str[i + 1]);
			var_name = ft_substr(str, i + 1, var_len);
			var_value = get_var_value(var_name, ctx);
			result = append_string(result, var_value);
			free(var_name);
			free(var_value);
			i += var_len + 1;
		}
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}

/* ========================================================================== */
/*                    PROCESS TOKEN (EXPAND + UNQUOTE)                       */
/* ========================================================================== */

/**
 * process_token_value - Processa un valore di token: rimuove quote ed espande variabili
 * @value: Valore del token (può avere quote)
 * @ctx: Contesto della shell
 * 
 * Algoritmo:
 * 1. Se ha singole quote ('...') -> rimuove quote, NON espande variabili
 * 2. Se ha doppie quote ("...") -> rimuove quote, espande variabili
 * 3. Se non ha quote -> espande variabili
 * 
 * Return: Stringa processata (allocata)
 */
char	*process_token_value(char *value, t_shell_context *ctx)
{
	char	*temp;
	char	*result;
	int		len;

	if (!value)
		return (ft_strdup(""));
	len = ft_strlen(value);
	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
	{
		return (remove_quotes(value));
	}
	else if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
	{
		temp = remove_quotes(value);
		result = expand_variables(temp, ctx, 1);
		free(temp);
		return (result);
	}
	else
	{
		return (expand_variables(value, ctx, 0));
	}
}

