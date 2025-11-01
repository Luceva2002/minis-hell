/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:05:45 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/18 18:35:42 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*p;

	len = 0;
	i = 0;
	while (*s)
	{
		len++;
		s++;
	}
	p = malloc (len + 1);
	if (p == NULL)
		return (NULL);
	s -= len;
	while (i < len)
	{
		*p = *s;
		s++;
		p++;
		i ++;
	}
	*p = '\0';
	return (p - len);
}
