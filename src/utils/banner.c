/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 00:00:00 by luevange          #+#    #+#             */
/*   Updated: 2025/12/19 15:51:27 by luevange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_banner(void)
{
	ft_putstr_fd("\033[1;31m", 1);
	ft_putendl_fd("", 1);
	ft_putendl_fd("  __  __ ___ _   _ ___ ____  _   _ _____ _     _", 1);
	ft_putendl_fd(" |  \\/  |_ _| \\ | |_ _/ ___|| | | | ____| |   | |", 1);
	ft_putendl_fd(" | |\\/| || ||  \\| || |\\___ \\| |_| |  _| | |   | |", 1);
	ft_putendl_fd(" | |  | || || |\\  || | ___) |  _  | |___| |___| |___", 1);
	ft_putendl_fd(" |_|  |_|___|_| \\_|___|____/|_| |_|_____|_____|_____|", 1);
	ft_putstr_fd("\033[0m", 1);
	ft_putendl_fd("", 1);
	ft_putendl_fd("  Welcome to MINIS-HELL - Left all bugs here!", 1);
	ft_putendl_fd("", 1);
}
