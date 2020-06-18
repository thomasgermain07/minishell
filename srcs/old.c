/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:52:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/18 15:55:41 by thgermai         ###   ########.fr       */
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

static void		handle_fd(int n, int fd[n][2], int option)
{
	int i;

	i = -1;
	if (option == 0)
	{
		while (++i < n)
		{
			close(fd[i][0]);
			close(fd[i][1]);
		}
	}
	else if (option == 1)
	{
		while (++i < n)
			if (pipe(fd[i]) == -1)
				exit(1);
	}
}

static void		exec_pipes(t_call *calls, int n_pipe)
{
	int			fd[n_pipe][2];
	int			i;
	char		**call;
	int			j;

	handle_fd(n_pipe, fd, 1);
	i = -1;
	while (++i < n_pipe + 1 && (j = -1))
	{
		call = ft_split((calls + i)->str, ' ');
		if (fork() == 0)
		{
			/* input */
			if ((calls + i)->in != -1)
			{
				dup2((calls + i)->in, 0);
				close((calls + i)->in);
			}
			else if (i != 0)
				dup2(fd[i - 1][0], 0);
			/* output */
			if ((calls + i)->out != -1)
			{
				dup2((calls + i)->out, 1);
				close((calls + i)->out);
			}
			else if (i != n_pipe)
				dup2(fd[i][1], 1);
			handle_fd(n_pipe, fd, 0);
			execvp(call[0], call); // a remplacer par un parsing des functions
			ft_printf_e("function a foire\n");
		}
		while (call[++j])
			free(call[j]);
		free(call);
	}
	handle_fd(n_pipe, fd, 0);
	i = -1;
	while (++i < n_pipe + 1)
		wait(NULL);
}

void			check_pipes(char *args)
{
	t_call		calls[get_n_pipes(args, 0) + 2];
	int			i;
	int			last_i;

	i = 0;
	last_i = 0;
	while (get_n_pipes((args + last_i), 1))
	{
		calls[i].in = -1;
		calls[i].out = -1;
		calls[i].str = ft_substr(args + last_i, 0, get_n_pipes((args + last_i), 1));
		last_i += ft_strlen(calls[i].str) + 1;
		i++;
	}
	calls[i].str = ft_substr(args + last_i, 0, ft_strlen(args + last_i));
	calls[i].in = -1;
	calls[i].out = -1;
	calls[i + 1].str = NULL;
	configure_calls(calls);
	exec_pipes(calls, get_n_pipes(args, 0));
	i = -1;
	while (calls[++i].str)
	{
		free(calls[i].str);
		if (calls[i].in != -1)
			close(calls[i].in);
		if (calls[i].out != -1)
			close(calls[i].out);
	}
}
