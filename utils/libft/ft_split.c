/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 13:44:05 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/27 12:23:41 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static	size_t	count_words(const char *s, char c)
{
	size_t	n;

	n = 0;
	if (s == NULL)
		return (0);
	while (*s != '\0')
	{
		if (*s == c)
			s++;
		else
		{
			n++;
			while (*s != '\0' && *s != c)
				s++;
		}
	}
	return (n);
}

char	*first_word(char *str, char c)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (*str == '\0')
		return (NULL);
	while (str[i] != c && str[i] != '\0')
		i++;
	result = malloc (i + 1);
	if (result == NULL)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j ++;
	}
	result[j] = '\0';
	return (result);
}

char	*move_me(char *s, char c, int i)
{
	if (i == 0)
	{
		while (*s == c)
			s++;
	}
	else
	{
		if (i == 1)
		{
			while (*s != c && *s != '\0')
				s ++;
		}
	}
	return (s);
}

char	**sp0_free(int n, char **m, int i)
{
	char	**r;

	if (n == 0)
	{
		r = malloc (sizeof(char *));
		if (r == NULL)
			return (NULL);
		r [0] = NULL;
		return (r);
	}
	else
	{
		while (i > 0)
		{
			free (m [i - 1]);
			i --;
		}
		free (m);
		return (NULL);
	}
}

char	**ft_split(char const *s, char c)
{
	size_t	nb_words;
	size_t	i;
	char	*str;
	char	**matrice;

	nb_words = count_words(s, c);
	i = 0;
	matrice = NULL;
	str = (char *)s;
	if (nb_words == 0)
		return (sp0_free(nb_words, matrice, i));
	matrice = malloc (sizeof(char *) * (nb_words + 1));
	if (matrice == NULL)
		return (NULL);
	while (nb_words > i)
	{
		str = move_me(str, c, 0);
		matrice [i] = first_word(str, c);
		if (matrice [i] == NULL)
			return (sp0_free(nb_words, matrice, i));
		str = move_me(str, c, 1);
		i ++;
	}
	matrice [i] = NULL;
	return (matrice);
}
 
