/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:37:38 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/27 13:19:07 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*my_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		*p = '\0';
		n --;
		p ++;
	}
	return (s);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total;
	void	*p;

	if (size != 0 && nmemb >= ((size_t) - 1) / size)
		return (NULL);
	total = nmemb * size;
	p = malloc (total);
	if (p == NULL)
		return (NULL);
	my_bzero(p, total);
	return (p);
}
