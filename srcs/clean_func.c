/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 23:43:21 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/21 14:07:27 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			clean_calls(t_call *calls)
{
	int			i;

	i = -1;
	while ((calls +  ++i)->str)
		free((calls + i)->str);
}

void			close_pipes(int pipes[][2], int size)
{
	int			i;

	i = -1;
	while (++i < size)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
