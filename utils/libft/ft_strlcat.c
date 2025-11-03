/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:29:53 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/30 12:07:19 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	n;
	size_t	i;
	size_t	k;

	n = ft_strlen(dst);
	k = ft_strlen(src);
	i = 0;
	if (size <= n)
		return (size + k);
	while ((i + n + 1) < size && src[i] != '\0')
	{
		dst[n + i] = src[i];
		i ++;
	}
	dst[n + i] = '\0';
	return (n + k);
}
/*
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d;
	size_t	s;
	size_t	i;

	if (!dst && size == 0)
		return (ft_strlen(src));
	d = 0;
	s = ft_strlen(src);
	i = 0;
	while (dst[d] && d < size)
		d++;
	if (d >= size)
		return (size + s);
	while (src[i] && (d + i + 1) < size)
	{
		dst[d + i] = src[i];
		i ++;
	}
	dst[d + i] = '\0';
	return (d + s);
}*/
