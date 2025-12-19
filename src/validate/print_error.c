/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:40:00 by luevange          #+#    #+#             */
/*   Updated: 2025/11/03 01:30:00 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syntax_print_error(int error_code)
{
	if (error_code == SYNTAX_UNCLOSED)
		ft_printf("minishell: syntax error: unclosed quotes\n");
	else if (error_code == SYNTAX_OPERATOR)
		ft_printf("minishell: syntax error near unexpected token\n");
	else if (error_code == SYNTAX_PARENS)
		ft_printf("minishell: syntax error: unmatched parentheses\n");
}
