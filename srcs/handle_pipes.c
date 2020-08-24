/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:31:11 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/24 15:05:11 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				create_pipes(t_call *calls, int pipes[][2])
{
	int			i;
	int			size;

	i = 0;
	while (calls[i].str)
		i++;
	size = --i;
	if (size == 0)
		return (0);
	i = -1;
	while (++i < size)
	{
		if (pipe(pipes[i]) == -1)
		{
			ft_printf_e("Error: pipe creation failed\n");
			exit(0);
		}
	}
	return (size);
}

void			connect_pipes(t_call *calls, int pipes[][2], int n_pipes)
{
	int			i;

	i = -1;
	while (calls[++i].str)
	{
		if (calls[i].in != 0)
			;
		else if (calls[i].in == 0 && i != 0)
			calls[i].in = pipes[i - 1][0];
		if (calls[i].out != 1)
			;
		else if (calls[i].out == 1 && i < n_pipes && pipes[i][0])
			calls[i].out = pipes[i][1];
	}
}
