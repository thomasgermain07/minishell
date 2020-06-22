/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 11:13:04 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/22 15:34:00 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*find_path(char **paths, char *bin)
{
	char			*path;
	int				i;
	struct stat		stats;

	i = -1;
	while (paths[++i])
	{
		if (i == 0)
			path = ft_strjoin(paths[i] + 5, "/");
		else
			path = ft_strjoin(paths[i], "/");
		path = ft_strjoin_f1(path, bin);
		if (stat(path, &stats) != -1)
			return (path);
		free(path);
	}
	return (NULL);
}

static char		*get_path(t_call *call, char *bin)
{
	char		**paths;
	char		*path;
	int			i;

	i = -1;
	while (call->env[++i])
		if (!ft_strncmp(call->env[i], "PATH", 4))
			break ;
	if (!call->env[i])
	{
		ft_printf_e("Error: path doesn't exist\n");
		exit(3);
	}
	paths = ft_split(call->env[i], ':');
	if (!(path = find_path(paths, bin)))
		ft_printf_e("Error: %s: unkown binary\n", bin);
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	free(bin);
	return (path);
}

void			exec_binary(t_call	*call, int pipes[][2], int size)
{
	char		**func;
	int			i;

	i = -1;
	func = ft_split(call->str, ' '); // A remplacer par un parse d'arg : "", etc
	func[0] = get_path(call, func[0]);
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
		execve(func[0], func, call->env); // A remplacer par un parse de function
		ft_printf_e("Error: execvp: %s\n", strerror(errno));
	}
	while (func[++i])
		free(func[i]);
	free(func);
}
