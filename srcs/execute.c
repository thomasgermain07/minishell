/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:47:56 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/03 09:51:49 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t			exec1(t_call *call, int pipes[][2], int size)
{
	char		**func;
	int			i;
	pid_t		pid;
	char		**env_var;

	i = -1;
	env_var = list_to_tab(call->env);
	func = parse_func(call->str, call->env);
	if (!known_func(func[0]))
		if (!(func[0] = parse_exec(call, func[0])))
			return (-1);
	if ((pid = fork()) == 0)
	{
		duplicate_fd(call);
		close_pipes(pipes, size);
		exit(execute(call, func, env_var));
	}
	clean_array(func);
	free(env_var);
	return (pid);
}

void			exec2(t_call *call)
{
	char		**func;
	char		**var_env;
	pid_t		pid;

	var_env = list_to_tab(call->env);
	func = parse_func(call->str, call->env);
	if (known_func(func[0]))
	{
		exec_knonw(call, func, var_env);
		return ;
	}
	func[0] = parse_exec(call, func[0]);
	if ((pid = fork()) == 0)
	{
		duplicate_fd(call);
		execve(func[0], func, var_env);
		ft_printf_e("Minishell: execve: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	clean_array(func);
	free(var_env);
	wait_pids(&pid, 1, call);
}
