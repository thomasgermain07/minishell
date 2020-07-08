/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_utiles.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:52:48 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 22:54:06 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char			*get_key(char *str)
{
	int			i;

	i = -1;
	while (str[++i])
		if (str[i] == '=')
			break ;
	return (ft_substr(str, 0, i + 1));
}

int				add_env(t_call *call, char *key, char *value, int option)
{
	char		*var;
	t_list		*current;

	var = ft_strjoin(key, value);
	current = *call->env;
	if (option == 0)
		ft_lstadd_back(call->env, ft_lstnew(var));
	else
	{
		while (current)
		{
			if (!ft_strncmp(key, (char *)current->content, ft_strlen(key)))
			{
				free(current->content);
				current->content = (void *)var;
				break ;
			}
			current = current->next;
		}
	}
	if (current)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int				delete_element(t_call *call, char *key)
{
	t_list		*current;
	t_list		*previous;

	current = *call->env;
	previous = current;
	while (current)
	{
		if (!ft_strncmp(key, (char *)current->content, ft_strlen(key)))
		{
			if (previous == current)
				call->env = &(current->next);
			else
				previous->next = current->next;
			ft_lstdelone(current, free);
			return (EXIT_SUCCESS);
		}
		previous = current;
		current = current->next;
	}
	return (EXIT_FAILURE);
}
