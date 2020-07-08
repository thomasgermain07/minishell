/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:45:09 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 22:46:13 by thgermai         ###   ########.fr       */
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

char			*parse_exec(t_call *call, char *bin)
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
