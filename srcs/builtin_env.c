/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:24:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/01 11:02:33 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*get_key(char *str)
{
	int			i;

	i = -1;
	while (str[++i])
		if (str[i] == '=')
			break ;
	return (ft_substr(str, 0, i + 1));
}

static int		add_env(t_call *call, char *key, char *value, int option)
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

static int		delete_element(t_call *call, char *key)
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

int				ft_unset(t_call *call, char **func)
{
	int			i;
	char		*key;

	i = 0;
	while (func[++i])
	{
		key = ft_strjoin(func[i], "=");
		printf("%s\n", key);
		if (find_value(key, call->env))
			delete_element(call, key);
		free(key);
	}
	return (EXIT_SUCCESS);
}

int				ft_env(t_call *call)
{
	t_list		*current;

	if (call->env)
	{
		current = *call->env;
		while(current)
		{
			write(1, (char *)current->content, ft_strlen((char*)current->content));
			write(1, "\n", 1);
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}

int				ft_export(t_call *call, char **func)
{
	int			i;
	char		*key;

	i = 0;
	while (func[++i])
	{
		if (func[i][0] != '=' && ft_strchr(func[i], '='))
		{
			key = get_key(func[i]);
			if (find_value(key, call->env))
				add_env(call, key, ft_strchr(func[i], '=') + 1, 1);
			else
				add_env(call, key, ft_strchr(func[i], '=') + 1, 0);
			free(key);
		}
	}
	return (EXIT_SUCCESS);
}
