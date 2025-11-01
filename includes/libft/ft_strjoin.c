/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 20:41:49 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/23 19:08:18 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		k;
	char	*p;

	k = 0;
	j = ft_strlen(s1);
	i = ft_strlen(s2) +j;
	p = malloc (i + 1);
	if (p == NULL)
		return (NULL);
	while (k < j)
	{
		p[k] = s1[k];
		k ++;
	}
	j = 0;
	while (k < i)
	{
		p[k] = s2[j];
		k ++;
		j ++;
	}
	p[k] = '\0';
	return (p);
}
/*#include <stdio.h>

int main(void)
{
    char *s1 = "Hello, ";
    char *s2 = "World!";
    char *result = ft_strjoin(s1, s2);

    if (result)
    {
        printf("Result: %s\n", result); // Output: "Hello, World!"
        free(result); // Don't forget to free the memory!
    }

    return 0;
}*/
