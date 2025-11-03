/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 00:22:38 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 00:22:39 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strndup(const char *s, size_t n)
{
    char	*dest;
    size_t	i;

    i = 0;
    dest = malloc(sizeof(char) * n + 1);
    if (!dest)
        return (NULL);
    while (i < n)
    {
        dest[i] = s[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}
