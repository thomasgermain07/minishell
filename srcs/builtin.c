/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 21:54:49 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/12 23:47:36 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				ft_echo(char **func)
{
	int			i;
	int			space;

	i = 0;
	space = 1;
	while (func[++i])
	{
		if (ft_strncmp(func[i], "-n", 3) == 0)
			space = 0;
		else
		{
			if (write(1, func[i], ft_strlen(func[i])) == -1)
			{
				ft_printf_e("%s\n", strerror(errno));
				return (EXIT_FAILURE);
			}
			if (func[i + 1])
				write(1, " ", 1);
		}
	}
	if (space)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int				ft_cd(t_call *call, char **func) // ne marche pas encore j'y travaille
{
	char		*old_pwd;

	old_pwd = ft_strdup(find_value("PWD=", call->env) + 4);
	if (func[1])
	{
		if (ft_strlen(func[1]) == 1 && func[1][0] == '-')
		{
			if (chdir(find_value("OLDPWD=", call->env) + 7) != -1)
				ft_printf("%s\n", find_value("OLDPWD=", call->env) + 7);
		}
		else if (chdir(func[1]) == -1)
		{
			ft_printf_e("Minishell: cd: %s: %s\n", func[1], strerror(errno));
			free(old_pwd);
			return (EXIT_FAILURE);
		}
	}
	else
		chdir(find_value("HOME=", call->env) + 5);
	add_env(call, "PWD=", get_cwd(), 1);
	add_env(call, "OLDPWD=", old_pwd, 1);
	free(old_pwd);
	return (EXIT_SUCCESS);
}

int				ft_pwd(void)
{
	char		buf[512];
	char		*current;

	if ((current = getcwd(buf, 512)) == NULL)
	{
		ft_printf_e("Minishell: error: %s\n", strerror(errno));
		return (EXIT_FAILURE);
	}
	ft_printf("%s\n", current);
	return (EXIT_SUCCESS);
}

int				ft_builtin_exit(int *exit_info)
{
	int i;

	i = -1;
	if (g_pids)
		while (g_pids[++i])
			kill(g_pids[i], 2);
	*exit_info = 1;
	return (EXIT_SUCCESS);
}
