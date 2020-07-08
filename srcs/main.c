/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 23:12:51 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	g_pids = NULL;
	if (signal(SIGINT, control_c) == SIG_ERR)
	{
		ft_printf_e("%s\n", "Could not set signal handler");
		return (EXIT_FAILURE);
	}
	if (signal(SIGQUIT, control_quit) == SIG_ERR)
	{
		ft_printf_e("%s\n", "Could not set signal handler");
		return (EXIT_FAILURE);
	}
	prompt(env);
	return (0);
}
