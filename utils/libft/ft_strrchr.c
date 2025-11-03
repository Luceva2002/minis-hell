/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:05:52 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/21 03:10:25 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*end;

	end = NULL;
	while (*s != '\0')
	{
		if (*s == (char)c)
			end = (char *)s;
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (end);
}
