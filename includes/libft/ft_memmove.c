/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:56:43 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/21 02:59:46 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t num)
{
	const char	*s;
	char		*d;

	s = (const char *)src;
	d = (char *)dest;
	if (d > s && d < (s + num))
	{
		d += num;
		s += num;
		while (num--)
			*(--d) = *(--s);
	}
	else
	{
		while (num --)
			*d++ = *s++;
	}
	return (dest);
}
