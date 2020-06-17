/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:52:31 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/17 12:08:14 by thgermai         ###   ########.fr       */
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

static void		exec_process(char **func, int t_pro, int n_pro, int fd[t_pro - 1][2])
{
	if (n_pro == 0)
	{
		close(fd[0][0]);
		dup2(fd[0][1], 1);
		close (fd[0][1]);
	}
	else if (n_pro == t_pro - 1)
	{
		close(fd[n_pro - 1][1]);
		dup2(fd[n_pro - 1][0], 0);
		close(fd[n_pro - 1][0]);
	}
	else
	{
		close(fd[n_pro - 1][1]);
		close(fd[n_pro][0]);
		dup2(fd[n_pro - 1][0], 0);
		dup2(fd[n_pro][1], 1);
		close(fd[n_pro - 1][0]);
		close(fd[n_pro][1]);
	}
	int ret = execvp(func[0], func);
	printf("exec failed with error %d\n", ret);
}

static void		exec_pipes(char **tab, int n_pipe)
{
	char		**call;
	int			fd[n_pipe][2]; // fd[0] read | fd[1] write
	int			i;
	pid_t		pid;

	i = -1;
	while (++i < n_pipe)
		if (pipe(fd[i]) == -1)
			exit(1);
	i = -1;
	while (++i < n_pipe + 1)
	{
		call = ft_split(tab[i], ' ');
		if ((pid = fork()) == 0)
		{
			exec_process(call, n_pipe + 1, i, fd);
			printf("if printed there is a problem\n"); // del
		}
		else
			waitpid(pid, NULL, 0);
	}
	exit(0);
}

void			check_pipes(char *args)
{
	char		*str[get_n_pipes(args, 0) + 2];
	int			i;
	int			last_i;
	pid_t		pid;

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
	if ((pid = fork()) == 0)
		exec_pipes(str, get_n_pipes(args, 0));
	else
		waitpid(pid, NULL, 0);
}
