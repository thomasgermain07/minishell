/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles_func.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 23:43:21 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/25 14:43:34 by thgermai         ###   ########.fr       */
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
		ft_printf_e("Minishell: error: tab creation failed\n");
		exit(1);
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
		ft_printf_e("Minishell: error: list creation failed\n");
		exit(1);
	}
	*list = NULL;
	while (env[++i])
		ft_lstadd_back(list, ft_lstnew(env[i]));
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
		if (!ft_strncmp(str, (char *)current->content, ft_strlen(str)))
			return ((char *)current->content);
		current = current->next;
	}
	return (NULL);
}
