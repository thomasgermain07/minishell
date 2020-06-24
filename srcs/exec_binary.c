/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 11:13:04 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/25 00:56:11 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*fill_pwd(char *bin)
{
	char		*current_dir;
	char		*temp;

	current_dir = ft_strjoin(get_cwd(), "/");
	temp = ft_strjoin(current_dir, bin + 2);
	free(current_dir);
	free(bin);
	return (temp);
}

static char		*find_path(char **paths, char *bin)
{
	char			*path;
	int				i;
	struct stat		stats;

	if (stat(bin, &stats) != -1)
		return (ft_strdup(bin));
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
	char		*var;
	int			i;

	i = -1;
	var = find_value("PATH=", call->env);
	if (!var)
	{
		ft_printf_e("Minishell: path variable doesn't exist\n");
		exit(3);
	}
	if (bin[0] && bin[1] && bin[0] == '.' && bin[1] == '/')
		bin = fill_pwd(bin);
	paths = ft_split(var, ':');
	if (!(path = find_path(paths, bin)))
		ft_printf_e("Minishell: %s: command not found\n", bin);
	i = -1;
	clean_array(paths);
	free(bin);
	return (path);
}

static void		duplicate_fd(t_call *call)
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

pid_t			exec_binary(t_call	*call, int pipes[][2], int size)
{
	char		**func;
	int			i;
	pid_t		pid;
	char		**env_var;

	i = -1;
	env_var = list_to_tab(call->env);
	func = parse_func(call->str);
	if (!(func[0] = get_path(call, func[0]))) // Serra appler dans le parse d'arg
		exit(2);
	if ((pid = fork()) == 0)
	{
		duplicate_fd(call);
		close_pipes(pipes, size);
		execve(func[0], func, env_var); // A remplacer par un parse de function
		ft_printf_e("Minishell: execvp: %s\n", strerror(errno));
	}
	clean_array(func);
	free(env_var);
	return (pid);
}
