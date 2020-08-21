/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:45:09 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/20 17:02:58 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*fill_pwd(char *bin)
{
	char		*current_dir;
	char		*temp;
	char		*cwd;

	cwd = ft_strdup(get_cwd());
	current_dir = ft_strjoin(cwd, "/");
	temp = ft_strjoin(current_dir, bin + 2);
	free(current_dir);
	free(bin);
	free(cwd);
	return (temp);
}

static char		*check_is_file(char *bin, char *original_bin)
{
	struct stat		stats;

	ft_memset(&stats, 0, sizeof(stats));
	if (stat(bin, &stats) != -1)
	{
		if (S_ISDIR(stats.st_mode))
		{
			ft_printf_e("bash: %s: is a directory\n", original_bin);
			return (ft_strdup(""));
		}
		return (ft_strdup(bin));
	}
	return (NULL);
}

static char		*find_path(char **paths, char *bin, char *original_bin)
{
	char			*path;
	int				i;
	struct stat		stats;
	char			*temp;

	temp = check_is_file(bin, original_bin);
	if (temp)
		return (temp);
	i = -1;
	if (paths)
	{
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
	}
	return (NULL);
}

static void		clean_exec(char **paths[], char **bin, char **original_bin)
{
	clean_array(*paths);
	free(*bin);
	free(*original_bin);
}

static int		is_dir(char *bin)
{
	if (ft_strlen(bin) == 1 && bin[0] == '.')
	{
		ft_printf("bash: .: filename argument required .:\n"); // a modifier plus tard
		ft_printf("usage: . filename [arguments]\n");
		return (-1);
	}
	else if (!ft_strncmp(bin, "/", 1) || !ft_strncmp(bin, "./", 2))
		return (1);
	return (0);
}

char			*parse_exec(t_call *call, char *bin) // return null si probleme
{
	char		**paths;
	char		*original_bin;
	char		*path;
	char		*var;
	int			ret;

	path = NULL;
	if ((ret = is_dir(bin)) == -1)
		return (NULL);
	if (bin[0] && bin[1] && bin[0] == '.' && bin[1] == '/')
		bin = fill_pwd(bin);
	if (var == NULL)
		paths = NULL;
	else
		paths = ft_split(var, ':');
	if (!ft_strncmp(original_bin, "..", 3))
		ft_printf_e("bash: ..: Command not found\n");
	else if (!(path = find_path(paths, bin, original_bin)))
		ft_printf_e("bash: ici 1: %s: %s\n", original_bin, strerror(errno));
	if (path && !ft_strlen(path))
	{
		free(path);
		clean_exec(&paths, &bin, &original_bin);
		return (NULL);
	}
	clean_exec(&paths, &bin, &original_bin);
}
