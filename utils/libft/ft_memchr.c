/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 10:48:43 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/18 18:10:55 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*m;

	m = (const unsigned char *)s;
	while (n > 0 && *m != (unsigned char)c)
	{
		n --;
		m ++;
	}
	if (n == 0)
		return (NULL);
	return ((void *)m);
}
