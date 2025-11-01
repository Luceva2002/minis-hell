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
