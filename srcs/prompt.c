/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:27:19 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/03 10:38:41 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			save_ret(t_list **env, int value)
{
	t_list		*current;

	current = *env;
	if (ft_strncmp("?=", (char *)current->content, 2))
		ft_lstadd_front(env, ft_lstnew(ft_strjoin_f2("?=", ft_itoa(value))));
	else
	{
		free(current->content);
		current->content = ft_strjoin_f2("?=", ft_itoa(value));
	}
}

void			wait_pids(pid_t *pids, int size, t_call *calls)
{
	int			i;
	int			status;

	i = -1;
	while (++i < size && pids[i] != -1)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			calls[i].ret = WEXITSTATUS(status);
			save_ret(calls->env, calls[i].ret);
		}
	}
}

static void		exec_input(char *str, t_list **env)
{
	t_call		calls[get_n_pipes(str, 0) + 2];
	int			pipes[get_n_pipes(str, 0)][2];
	int			i;

	i = -1;
	if (!(g_pids = malloc(sizeof(pid_t) * (get_n_pipes(str, 0) + 2))))
	{
		ft_printf_e("Minishell: error: malloc failed\n");
		return ;
	}
	g_pids[get_n_pipes(str, 0) + 1] = 0;
	parse_pipes(str, calls);
	while (calls[++i].str)
		parse_call(&calls[i], env);
	if (i > 1)
	{
		create_pipes(calls, pipes);
		i = -1;
		while (calls[++i].str)
			connect_pipes(calls, pipes);
		i = -1;
		while (calls[++i].str)
			g_pids[i] = exec1(&calls[i], pipes, get_n_pipes(str, 0));
		close_pipes(pipes, get_n_pipes(str, 0));
		wait_pids(g_pids, get_n_pipes(str, 0) + 1, calls);
	}
	else
		exec2(&calls[0]);
	clean_calls(calls);
	free(g_pids);
}

void			print(void)
{
	ft_printf("\033[1;32mMINISHELL \033[0m 👉 ");
}

void			prompt(char **env)
{
	char		*args;
	t_list		**list;
	char 		**split_args;
	int			i;

	list = tab_to_list(env);
	save_ret(list, 0);
	args = NULL;
	split_args = NULL;
	i = -1;
	while (1)
	{
		g_pids = NULL;
		print();
		if (get_next_line(0, &args) == 0)
			if (!(control_d()))
				exit(0);
		if (!ft_strncmp(args, "exit", 5)) // a enlever quand on aura fait le vrai
			break ;
		if (ft_strlen(args))
		{
			i = -1;
			split_args = parse_semicolon(args);
			if (split_args)
				while (split_args[++i])
					exec_input(split_args[i], list);
		}
		free(args);
		clean_array(split_args);
	}
	ft_lstclear(list, &free);
	system("leaks minishell");
}
