/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:24:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/09 18:45:04 by thgermai         ###   ########.fr       */
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

int				ft_env(t_call *call, char **func)
{
	t_list		*current;
	char		*key;

	if (func[1])
		return (EXIT_FAILURE);
	if (call->env)
	{
		current = *call->env;
		while (current)
		{
			key = get_key((char *)current->content);
			if (ft_strlen(find_value(key, call->env) + ft_strlen(key)))
			{
				write(1, (char *)current->content,
					ft_strlen((char*)current->content));
				write(1, "\n", 1);
			}
			free(key);
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}

int				ft_env1(t_call *call)
{
	t_list		*current;
	t_list		**sorted_list;
	char		*key;

	sorted_list = sort_var_env(call->env);
	current = *sorted_list;
	while (current)
	{
		write(1, "declare -x ", 11);
		key = get_key((char *)current->content);
		write(1, key, ft_strlen(key) - 1);
		if (ft_strlen(find_value(key, sorted_list) + ft_strlen(key)))
			ft_printf("=\"%s\"\n", find_value(key, call->env) + ft_strlen(key));
		else
			write(1, "\n", 1);
		free(key);
		current = current->next;
	}
	ft_lstclear(sorted_list, &free);
	free(sorted_list);
	return (EXIT_SUCCESS);
}

int				ft_export(t_call *call, char **func) // reste a verifier si les key n'est que alphanum
{
	int			i;
	char		*key;

	i = 0;
	if (func[1] == NULL)
		ft_env1(call);
	while (func[++i])
	{
		if (func[i][0] == '=')
		{
			ft_printf_e("minishell: export: '%s': not a valid identifier\n", func[i]);
			return (EXIT_FAILURE);
		}
		if (func[i][0] != '=')
		{
			key = get_key(func[i]);
			if (find_value(key, call->env))
			{
				if (ft_strchr(func[i], '='))
					add_env(call, key, ft_strchr(func[i], '=') + 1, 1);
			}
			else if (ft_strchr(func[i], '='))
				add_env(call, key, ft_strchr(func[i], '=') + 1, 0);
			else
				add_env(call, key, "\0", 0);
			free(key);
		}
	}
	return (EXIT_SUCCESS);
}
