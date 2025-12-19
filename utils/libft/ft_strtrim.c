/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:10:02 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/23 19:48:20 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	found(char c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		set ++;
	}
	return (0);
}

char	*exept(void)
{
	char	*r;

	r = (char *)malloc(1 * sizeof(char));
	r[0] = '\0';
	return (r);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		len;
	int		j;
	char	*r;

	i = 0;
	j = 0;
	len = 0;
	while (found (s1[i], set) == 1)
		i ++;
	while (s1[len])
		len++;
	len --;
	while (found (s1[len], set) == 1)
		len --;
	len = len - i + 1;
	if (len <= 0)
		return (exept());
	r = malloc (len + 1);
	if (r == NULL)
		return (NULL);
	while (j < len)
		r[j++] = s1[i++];
	r[j] = '\0';
	return (r);
}
