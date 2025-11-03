/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:47:24 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/19 03:11:18 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ps;
	const unsigned char	*ss;

	ps = (const unsigned char *)s1;
	ss = (const unsigned char *)s2;
	while (n > 0)
	{
		if (*ps != *ss)
			return (*ps - *ss);
		ps ++;
		ss ++;
		n --;
	}
	return (0);
}
