/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/22 14:47:21 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			parse_input(char *str, char **env)
{
	t_call		calls[get_n_pipes(str, 0) + 2];
	int			pipes[get_n_pipes(str, 0)][2];
	int			i;

	i = -1;
	check_pipes(str, calls);
	while (calls[++i].str)
		parse_call(&calls[i], env);
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
}

void			prompt(char **env)
{
	char *args;

	args = NULL;
	while (1)
	{
		ft_printf("MINISHELL -> ");
		get_next_line(0, &args);
		parse_input(args, env);
		free(args);
	}
}

int			main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	prompt(env);
	return (0);
}
