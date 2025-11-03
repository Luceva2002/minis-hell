/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:24:00 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/21 02:58:26 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*r;
	unsigned char	*s;

	r = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (n > 0)
	{
		*r = *s;
		s ++;
		r ++;
		n--;
	}
	return (dest);
}
