/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:41:59 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/19 02:39:02 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*p;
	char	v;

	p = (char *)s;
	v = (char)c;
	while (n > 0)
	{
		*p = v;
		p++;
		n--;
	}
	return (s);
}
/*
int main() {
    char buffer[10];

    // Fill the buffer with '-'
    memset(buffer, '-', sizeof(buffer));
    buffer[9] = '\0'; // Null-terminate for printing

    printf("Buffer: %s\n", buffer); // Output: ---------

    return 0;
}*/
