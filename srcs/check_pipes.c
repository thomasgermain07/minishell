/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 21:49:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/18 23:42:52 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		get_n_pipes(char *args, int option)
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

t_call			**check_pipes(char *str)
{
	t_call		**calls;
	int			i;
	int			last_i;

	last_i = 0;
	i = get_n_pipes(str, 0) + 1;
	if (!(calls = malloc(sizeof(t_call *) * (i + 1))))
		exit(1);
	while (i > 0)
		if (!(calls[--i] = malloc(sizeof(t_call))))
			exit(1);
	while (get_n_pipes(str + last_i, 1))
	{
		calls[i]->str = ft_substr(str + last_i, 0,
			get_n_pipes(str + last_i, 1));
		last_i += ft_strlen(calls[i]->str) + 1;
		i++;
	}
	calls[i]->str = ft_substr(str + last_i, 0, ft_strlen(str + last_i));
	calls[i + 1] = NULL;
	return (calls);
}

int				**create_pipes(t_call **calls)
{
	int			**fd;
	int			i;
	int			size;

	i = 0;
	while (calls[i])
		i++;
	size = --i;
	if (size == 0)
		return (NULL);
	if (!(fd = malloc(sizeof(int *) * (size + 1))))
		exit(1);
	while (i > 0)
		if (!(fd[--i] = malloc(sizeof(int[2]))))
			exit(1);
	while (i < size)
	{
		if (pipe(fd[i++]) == -1)
		{
			ft_printf_e("Error: pipe creation failed\n");
			exit(0);
		}
	}
	fd[i] = NULL;
	return (fd);
}
