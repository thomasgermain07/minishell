/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utiles.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:48:42 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 23:11:59 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			exec_knonw(t_call *call, char **func, char **var_env)
{
	int			fds[2];

	fds[0] = dup(0);
	fds[1] = dup(1);
	duplicate_fd(call);
	call->ret = execute(call, func, var_env);
	clean_array(func);
	free(var_env);
	dup2(fds[0], 0);
	dup2(fds[1], 1);
	close(fds[0]);
	close(fds[1]);
}

void			duplicate_fd(t_call *call)
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
}

int				execute(t_call *call, char **func, char **env)
{
	if (!ft_strncmp(func[0], "echo", 4))
		return (ft_echo(func));
	else if (!ft_strncmp(func[0], "cd", 2))
		return (ft_cd(func));
	else if (!ft_strncmp(func[0], "pwd", 3))
		return (ft_pwd());
	else if (!ft_strncmp(func[0], "export", 6))
		return (ft_export(call, func));
	else if (!ft_strncmp(func[0], "unset", 5))
		return (ft_unset(call, func));
	else if (!ft_strncmp(func[0], "env", 3))
		return (ft_env(call));
	else
	{
		execve(func[0], func, env);
		ft_printf_e("Minishell: execve: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
}
