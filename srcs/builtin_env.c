/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:24:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/09 14:41:30 by thgermai         ###   ########.fr       */
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

int				ft_env(t_call *call, int option)
{
	t_list		*current;
	int			i;

	if (call->env)
	{
		current = *call->env;
		while (current)
		{
			if (option == 1)
			{
				i = -1;
				write(1, "declare -x ", 11);
				while (((char *)current->content)[++i] != '=')
					write(1, &((char*)current->content)[i], 1);
				ft_printf("\"%s\"", &current->content[i]);
			}
			else
			{
				write(1, (char *)current->content,
				ft_strlen((char*)current->content));
			}
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
	if (func[1] == NULL)
		ft_env(call, 1);
	while (func[++i])
	{
		if (func[i][0] == '=')
		{
			ft_printf_e("minishell: export: '%s': not a valid identifier\n", func[i]);
			return (EXIT_FAILURE);
		}
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
