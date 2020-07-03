/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:24:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/03 10:36:10 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				ft_unset(t_call *call, char **func)
{
	int			i;
	char		*key;

	i = 0;
	while (func[++i])
	{
		key = ft_strjoin(func[i], "=");
		if (find_value(key, call->env))
			delete_element(call, key);
		free(key);
	}
	return (EXIT_SUCCESS);
}

int				ft_env(t_call *call)
{
	t_list		*current;
	int			i;

	i = 0;
	if (call->env)
	{
		current = *call->env;
		while (current)
		{
			if (i > -1)
			{
				write(1, (char *)current->content,
					ft_strlen((char*)current->content));
				write(1, "\n", 1);
			}
			current = current->next;
			i++;
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
