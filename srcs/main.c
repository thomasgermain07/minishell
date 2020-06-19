/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/19 14:47:36 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void			exec_binary(t_call	*call, int pipes[][2], int size)
{
	char		**func;
	int			i;
	int			j;

	i = -1;
	while((call + ++i)->str && (j = -1))
	{
		func = ft_split((call + i)->str, ' '); // A remplacer par un parse d'arg : "", etc
		if ((call + i)->in != 0)
			dup2((call + i)->in, 0);
		if ((call + i)->out != 1)
			dup2((call + i)->out, 1);
		if (fork() == 0)
		{
			close_pipes(pipes, size);
			execvp(func[0], func); // A remplacer par un parse de function
			ft_printf_e("Error: execvp failed\n");
		}
		while (func[++j])
			free(func[j]);
		free(func);
	}
	close_pipes(pipes, size);
	i = -1;
	while (++i < size + 1)
		wait(NULL);
}

void			parse_input(char *str)
{
	t_call		calls[get_n_pipes(str, 0) + 2];
	int			pipes[get_n_pipes(str, 0)][2];
	int			i;

	i = -1;
	check_pipes(str, calls);
	while (calls[++i].str)
		parse_call(&calls[i]);
	if (i > 0)
		create_pipes(calls, pipes);
	i = -1;
	while (calls[++i].str)
		connect_pipes(calls, pipes);
	exec_binary(calls, pipes, get_n_pipes(str, 0));
	//for (int x = 0; calls[x].str; x++)
		//printf("%s %d %d\n", calls[x].str, calls[x].in, calls[x].out);

	close_pipes(pipes, get_n_pipes(str, 0));
	clean_calls(calls);
	if (pipes[0])
		clean_pipes(pipes, get_n_pipes(str, 0));
}

void			prompt(void)
{
	char *args;

	args = NULL;
	while (1)
	{
		ft_printf("MINISHELL -> ");
		get_next_line(0, &args);
		parse_input(args);
		free(args);
	}
}

int			main(void)
{
	prompt();
	return (0);
}
