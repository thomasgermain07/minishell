/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing_alice-2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:52:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/17 12:50:40 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "stdio.h"

typedef struct {
	int side[2];
} t_pipe;

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

static void		exec_pipes(char **tab, int i, t_pipe *tab_pipe, int nb_pipes)
{
	char		**call;
	int j;

	j = 0;
		if (i == 0)
		{

			dup2(tab_pipe[i].side[1], 1);
			call = ft_split(tab[i], ' ');
			while (j < nb_pipes)
			{
				close(tab_pipe[j].side[0]);
				close(tab_pipe[j].side[1]);
				j++;
			}
			execvp(call[0], call);
		}
		else if (i == nb_pipes)
		{
			dup2(tab_pipe[i - 1].side[0], 0);
			call = ft_split(tab[i], ' ');
			while (j < nb_pipes)
			{
				close(tab_pipe[j].side[0]);
				close(tab_pipe[j].side[1]);
				j++;
			}
			execvp(call[0], call);
		}
		else
		{
			dup2(tab_pipe[i - 1].side[0], 0);
			dup2(tab_pipe[i].side[1], 1);
			call = ft_split(tab[i], ' ');
			while (j < nb_pipes)
			{
				close(tab_pipe[j].side[0]);
				close(tab_pipe[j].side[1]);
				j++;
			}
			execvp(call[0], call);
		}
}

void forks_pipes(int n, char **args)
{
	int i;
	int status;
	i = 0;

	t_pipe tab_pipe[n - 1];

	while (i < n - 1)
	{
		pipe(tab_pipe[i].side);
		i++;
	}
	i = 0;
	while (i < n)
	{
		if (fork() == 0)
			exec_pipes(args, i, tab_pipe, n - 1);
		//else
		//	wait(NULL);
		i++;
	}
	i = 0;
	while (i < n -1)
	{
		close(tab_pipe[i].side[0]);
		close(tab_pipe[i].side[1]);
		i++;
	}
	for (i = 0; i < n ; i++)
		wait(&status);
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
	if (get_n_pipes(args, 0) != 0)
		forks_pipes(get_n_pipes(args, 0) + 1, str);
	//	if (fork() == 0)
	//		exec_pipes(str);
//	else
//		wait(NULL);
}
