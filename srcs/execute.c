/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:47:56 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/24 15:21:33 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void		refresh_var_underscore(char **func, t_call *call)
{
	int			i;

	i = -1;
	while (func[++i])
		;
	if (!(!ft_strncmp(func[0], "export", ft_strlen(func[0])) && !func[1]))
	{
		if (find_value("_=", call->env, 1))
			add_env(call, "_=", func[i - 1], 1);
		else
			add_env(call, "_=", func[i - 1], 0);
	}
}

static int		check_call_in(int in)
{
	if (in == -1)
	{
		g_exit_status = 1;
		g_exit_nb = 1;
		return (0);
	}
	return (1);
}

static pid_t	exit_exec1(char ***func, char ***env_var,
	int *exit_info, pid_t pid)
{
	clean_array(*func);
	free(*env_var);
	if (*exit_info == 1)
		return (-1);
	return (pid);
}

pid_t			exec1(t_call *call, int pipes[][2], int size, int *exit_info)
{
	char		**func;
	pid_t		pid;
	char		**env_var;

	if (!(check_call_in(call->in)))
		return (-1);
	if (!(func = parse(call->str, call->env)))
		return (-1);
	refresh_var_underscore(func, call);
	if (!known_func(func[0]))
	{
		if (!(func[0] = parse_exec(call, func[0])))
		{
			clean_array(func);
			return (-1);
		}
	}
	env_var = list_to_tab(call->env);
	if ((pid = fork()) == 0)
	{
		duplicate_fd(call);
		close_pipes(pipes, size);
		exit(execute(call, func, env_var, exit_info));
	}
	return (exit_exec1(&func, &env_var, exit_info, pid));
}

static int		free_exec2(char ***var_env, char ***func)
{
	int i;

	i = 0;
	free(*var_env);
	i = 1;
	while ((*func)[i])
	{
		free((*func)[i]);
		i++;
	}
	free(*func);
	return (1);
}

static int		set_exec2(t_call *call, char ***func, char ***var_env)
{
	if (!(check_call_in(call->in)))
		return (0);
	if (!((*func) = parse(call->str, call->env)))
		return (0);
	refresh_var_underscore(*func, call);
	*var_env = list_to_tab(call->env);
	return (1);
}

void			exec2(t_call *call, int *exit_info)
{
	char		**func;
	char		**var_env;
	pid_t		pid;

	if (!(set_exec2(call, &func, &var_env)))
		return ;
	if (known_func(func[0]))
	{
		exec_knonw(call, func, var_env, exit_info);
		return ;
	}
	if (!(func[0] = parse_exec(call, func[0])))
		if (free_exec2(&var_env, &func))
			return ;
	if ((pid = fork()) == 0)
	{
		duplicate_fd(call);
		execve(func[0], func, var_env);
		exit(EXIT_FAILURE);
	}
	clean_array(func);
	free(var_env);
	if (*exit_info)
		return ;
	wait_pids(&pid, 1, call);
}
