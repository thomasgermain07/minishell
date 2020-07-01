/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 23:43:21 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/01 10:43:53 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			clean_calls(t_call *calls)
{
	int			i;

	i = -1;
	while (calls[++i].str)
		free(calls[i].str);
}

void			close_pipes(int pipes[][2], int size)
{
	int			i;

	i = -1;
	while (++i < size)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

char			*get_cwd(void)
{
	static char	buffer[512];
	char		*cwd;

	ft_bzero(buffer, 512);
	cwd = getcwd(buffer, 512);
	return (cwd);
}

char			**list_to_tab(t_list **lst)
{
	char		**tab;
	int			i;
	int			size;
	t_list		*current;

	size = ft_lstsize(*lst);
	i = -1;
	if (!(tab = malloc(sizeof(char *) * (size + 1))))
	{
		ft_printf_e("Minishell: error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	current = *lst;
	while (++i < size)
	{
		tab[i] = (char *)current->content;
		current = current->next;
	}
	tab[i] = NULL;
	return (tab);
}

t_list			**tab_to_list(char **env)
{
	int			i;
	t_list		**list;

	i = -1;
	if (!(list = malloc(sizeof(t_list *))))
	{
		ft_printf_e("Minishell: error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	*list = NULL;
	while (env[++i])
		ft_lstadd_back(list, ft_lstnew(ft_strdup(env[i])));
	return (list);
}

void			clean_array(char **array)
{
	int			i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

char			*find_value(char *str, t_list **env)
{
	t_list		*current;

	current = *env;
	while (current)
	{
		if (str[ft_strlen(str) - 1] == '=')
		{
			if (!ft_strncmp(str, (char *)current->content, ft_strlen(str)))
				return ((char *)current->content);
		}
		else
		{
			if (!ft_strncmp(str, (char *)current->content, ft_strlen(str)) &&
				((char *)current->content)[ft_strlen(str)] == '=')
				return ((char *)current->content);
		}
		current = current->next;
	}
	return (NULL);
}

int				known_func(char *str)
{
	if (!ft_strncmp(str, "echo", 4))
		return (1);
	else if (!ft_strncmp(str, "cd", 2))
		return (1);
	else if (!ft_strncmp(str, "pwd", 3))
		return (1);
	else if (!ft_strncmp(str, "export", 6))
		return (1);
	else if (!ft_strncmp(str, "unset", 5))
		return (1);
	else if (!ft_strncmp(str, "env", 3))
		return (1);
	return (0);
}
