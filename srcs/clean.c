/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:25:17 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 22:26:08 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			clean_calls(t_call *calls)
{
	int			i;

	i = -1;
	while (calls[++i].str)
		free(calls[i].str);
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

void			clean_array(char **array)
{
	int			i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}
