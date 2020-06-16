/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:52:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/16 16:30:43 by thgermai         ###   ########.fr       */
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

static void		exec_pipes(char **tab)
{
	int			fd[2]; // fd[0] read | fd[1] write
	char		**call;

	if (pipe(fd) == -1)
		exit(1);
	if (fork() == 0)
	{
		call = ft_split(tab[0], ' ');
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execvp(call[0], call);
	}
	else
	{
		call = ft_split(tab[1], ' ');
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execvp(call[0], call);
	}
}

void			check_pipes(char *args)
{
	char		*str[get_n_pipes(args, 0) + 2];
	int			i;
	int			last_i;

	i = 0;
	last_i = 0;
	while (get_n_pipes((args + last_i), 1))
	{
		str[i] = ft_substr(args + last_i, 0, get_n_pipes((args + last_i), 1));
		last_i += ft_strlen(str[i]) + 1;
		i++;
	}
	str[i] = ft_substr(args + last_i, 0, ft_strlen(args + last_i));
	str[i + 1] = NULL;
	if (fork() == 0)
		exec_pipes(str);
	else
		wait(NULL);
}
