/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 16:10:54 by atetu             #+#    #+#             */
/*   Updated: 2020/07/02 23:15:05 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			control_c(int sig)
{
	int i;

	(void)sig;
	if (!g_pids)
	{
		write(1, "\b\b  \b\b\n", 7);
		print(); // valeur de retour :1
	}
	else
	{
		i = -1;
		while(++i)
			kill(g_pids[i], sig); // ajouter 128 et num sig -> 130
		write(1, "\n", 1);
	}
}

int				control_d(void)
{
	int i;

	i = -1;
	if (!g_pids)
	{
    	write(1, "\b   \b\b", 6);
        ft_printf("exit\n");
        return(0); // valeur de retour 0
	}
	else
	{
		while(++i)
			kill(g_pids[i], 2);
		ft_printf("exit\n");
		return (0);
	}
	return (1);
}

void			control_quit(int sig)
{
	int i;

	i = -1;
	if (g_pids) // variable globale a modifier quand fd = 0
	{
		ft_printf("Quit: %d\n", sig);
		while(++i)
			kill(g_pids[i], 2); // valeur de retoru = 128 + sig -> !131
	}
	else
		write(1, "\b\b  \b\b", 6); // valeur de retour: 127
}
