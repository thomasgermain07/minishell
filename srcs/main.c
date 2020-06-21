/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/21 14:07:44 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void			exec_binary(t_call	*call, int pipes[][2], int size)
{
	char		**func;
	int			i;

	i = -1;
	func = ft_split(call->str, ' '); // A remplacer par un parse d'arg : "", etc
	if (fork() == 0)
	{
		if (call->in != 0)
		{
			dup2(call->in, 0);
			close(call->in);
		}
		if (call->out != 1)
		{
			dup2(call->out, 1);
			close(call->out);
		}
		close_pipes(pipes, size);
		execvp(func[0], func); // A remplacer par un parse de function
		ft_printf_e("Error: execvp: %s\n", strerror(errno));
	}
	while (func[++i])
		free(func[i]);
	free(func);
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
	i = -1;
	while (calls[++i].str)
		exec_binary(&calls[i], pipes, get_n_pipes(str, 0));
	close_pipes(pipes, get_n_pipes(str, 0));
	i = -1;
	while (++i < get_n_pipes(str, 0) + 1)
		wait(NULL);
	clean_calls(calls);
	if (pipes[0])
		close_pipes(pipes, get_n_pipes(str, 0));
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
