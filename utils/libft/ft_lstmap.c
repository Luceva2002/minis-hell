/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ligharbi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 09:26:53 by ligharbi          #+#    #+#             */
/*   Updated: 2024/12/30 09:27:19 by ligharbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	clear_lst(t_list *lst, void (*del)(void *))
{
	t_list	*tmp;

	tmp = NULL;
	while (lst)
	{
		del(lst -> content);
		tmp = lst -> next;
		free (lst);
		lst = tmp;
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*r;

	new_lst = malloc(sizeof(t_list));
	if (!new_lst)
		return (NULL);
	r = new_lst;
	while (lst)
	{
		new_lst -> content = f(lst -> content);
		lst = lst -> next;
		new_lst -> next = malloc(sizeof(t_list));
		if (!new_lst)
		{
			clear_lst(r, del);
			return (NULL);
		}
		new_lst = new_lst -> next;
	}
	new_lst -> content = NULL;
	return (r);
}
