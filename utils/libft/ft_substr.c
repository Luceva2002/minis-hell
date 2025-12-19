/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:19:56 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/23 18:54:13 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*exption(void)
{
	char	*r;

	r = malloc (1);
	if (!r)
		return (NULL);
	r[0] = '\0';
	return (r);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*r;
	size_t	i;
	size_t	cmp;

	cmp = ft_strlen(s);
	i = 0;
	if (!s)
		return (NULL);
	if (start >= cmp)
		return (exption ());
	if (len > cmp - start)
		len = cmp - start;
	r = malloc (len + 1);
	if (r == NULL)
		return (NULL);
	while (len > i && s[start + i] != '\0')
	{
		r[i] = s[start + i];
		i++;
	}
	r[i] = '\0';
	return (r);
}
