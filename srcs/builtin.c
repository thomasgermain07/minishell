/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 21:54:49 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/08 20:06:26 by thgermai         ###   ########.fr       */
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

int				ft_cd(char **func)
{
	char		*buf;
	int			i;

	buf = NULL;
	i = 0;
	if (func[1] && chdir(func[1]) == -1)
	{
		ft_printf_e("Minishell: cd: %s: %s\n", func[1], strerror(errno));
		return (EXIT_FAILURE);
	}
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
