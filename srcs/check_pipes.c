/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 21:49:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/22 10:58:38 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			get_n_pipes(char *args, int option)
{
	int			i;
	int			in_quote;
	int			n_pipes;

	in_quote = 0;
	i = 0;
	n_pipes = 0;
	while (args[++i])
	{
		if (args[i] == '"' && i > 0 && args[i - 1] != '\\')
			in_quote == 1 ? in_quote-- : in_quote++;
		if (args[i] == '|' && in_quote == 0)
		{
			if (option == 1)
				return (i);
			n_pipes++;
		}
	}
	if (option == 1)
		return (0);
	return (n_pipes);
}

void			check_pipes(char *str, t_call *calls)
{
	int			i;
	int			last_i;

	last_i = 0;
	i = 0;
	while (get_n_pipes(str + last_i, 1))
	{
		(calls + i)->str = ft_substr(str + last_i, 0,
			get_n_pipes(str + last_i, 1));
		last_i += ft_strlen((calls + i)->str) + 1;
		i++;
	}
	(calls + i)->str = ft_substr(str + last_i, 0, ft_strlen(str + last_i));
	(calls + (i + 1))->str = NULL;
}

void			create_pipes(t_call *calls, int pipes[][2])
{
	int			i;
	int			size;

	i = 0;
	while ((calls + i)->str)
		i++;
	size = --i;
	if (size == 0)
		return ;
	i = -1;
	while (++i < size)
	{
		if (pipe(pipes[i]) == -1)
		{
			ft_printf_e("Error: pipe creation failed\n");
			exit(0);
		}
	}
}

void			connect_pipes(t_call *calls, int pipes[][2])
{
	int			i;
	t_call		*call;

	i = -1;
	call = NULL;
	while ((calls + ++i)->str)
	{
		call = (calls + i);
		if (call->in != 0)
			;
		else if (call->in == 0 && i != 0)
			call->in = pipes[i - 1][0];
		if (call->out != 1)
			;
		else if (call->out == 1 && pipes[i + 1][0])
			call->out = pipes[i][1];
	}
}
