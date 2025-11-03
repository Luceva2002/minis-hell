/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 01:28:51 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/21 03:04:34 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	unsigned int	i;
	char			*array;

	len = 0;
	i = 0;
	if (!s || !f)
		return (NULL);
	while (s[len])
		len ++;
	array = (char *)malloc((len + 1) * sizeof(char));
	if (!array)
		return (NULL);
	while (i < len)
	{
		array[i] = f(i, s[i]);
		i ++;
	}
	array[len] = '\0';
	return (array);
}
