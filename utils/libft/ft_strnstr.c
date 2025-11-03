/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:22:48 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/27 08:38:10 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	found(const char *a, const char *b, size_t len)
{
	size_t	i;

	i = 0;
	while (b[i] && a[i] && i < len)
	{
		if (a[i] != b[i])
			return (0);
		i ++;
	}
	return (b[i] == '\0');
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	if (len == 0)
		return (NULL);
	while (i < len && big[i] != '\0')
	{
		if (found(big + i, little, len - i))
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
