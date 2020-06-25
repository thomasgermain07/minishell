/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/25 13:29:23 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void		wait_pids(pid_t *pids, int size, t_call *calls)
{
	int			i;
	int			status;

	i = -1;
	while (++i < size && pids[i] != -1)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			calls[i].ret = WEXITSTATUS(status);
	}
}

static void		parse_input(char *str, t_list **env)
{
	t_call		calls[get_n_pipes(str, 0) + 2];
	int			pipes[get_n_pipes(str, 0)][2];
	pid_t		pids[get_n_pipes(str, 0) + 1];
	int			i;

	i = -1;
	check_pipes(str, calls);
	while (calls[++i].str)
		parse_call(&calls[i], env);
	if (i > 1)
		create_pipes(calls, pipes);
	i = -1;
	while (calls[++i].str)
		connect_pipes(calls, pipes);
	i = -1;
	while (calls[++i].str)
		pids[i] = exec_binary(&calls[i], pipes, get_n_pipes(str, 0));
	close_pipes(pipes, get_n_pipes(str, 0));
	wait_pids(pids, get_n_pipes(str, 0) + 1, calls);
	clean_calls(calls);
}

void			prompt(char **env)
{
	char		*args;
	t_list		**list;

	list = tab_to_list(env);
	args = NULL;
	while (1)
	{
		ft_printf("\033[1;32mMINISHELL \033[0m 👉 ");
		get_next_line(0, &args);
		if (ft_strlen(args))
			parse_input(args, list);
		free(args);
	}
}

int				main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	prompt(env);
	return (0);
}
