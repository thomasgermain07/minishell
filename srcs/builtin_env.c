/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/30 17:24:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/08/24 15:29:51 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void		unset_env_var(char *key)
{
	if (!(ft_strncmp(key, "PWD=", 5)))
	{
		free(g_pwd);
		g_pwd = ft_strdup("");
	}
	if (!(ft_strncmp(key, "OLDPWD=", 8)))
	{
		free(g_oldpwd);
		g_oldpwd = ft_strdup("");
	}
}

static void		unset_error_message(char c)
{
	ft_printf_e("bash: line 1: unset: "); // minishell: unset:
	ft_printf_e("`%c': not a valid identifier\n", c);
}

int			ft_unset(t_call *call, char **func)
{
	int			i;
	char		*key;
	int			j;

	i = 0;
	while (func[++i])
	{
		j = -1;
		while (func[i][++j])
		{
			if (!ft_isalnum((int)func[i][j]) && func[i][j] != '_')
			{
				unset_error_message(func[i][j]);
				return ((g_exit_nb = EXIT_FAILURE));
			}
		}
		key = ft_strjoin(func[i], "=");
		unset_env_var(key);
		if (find_value(key, call->env, 1))
			delete_element(call, key);
		if (find_value(key, call->env, 2))
			delete_element(call, func[i]);
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
			if (((char *)current->content)[ft_strlen(key) - 1] == '=')
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

static void		write_env1(char *key, t_call *call, t_list *current)
{
	char		*to_print;

	to_print = NULL;
	write(1, "declare -x ", 11);
	write(1, key, ft_strlen(key) - 1);
	if (ft_strchr((char *)current->content, '='))
	{
		to_print = find_value(key, call->env, 1) + ft_strlen(key);
		ft_printf("=\"%s\"\n", to_print);
	}
	else
		write(1, "\n", 1);
	free(key);
}

static int		ft_env1(t_call *call)
{
	t_list		*current;
	t_list		**sorted_list;
	char		*key;
	char		*temp;

	sorted_list = sort_var_env(call->env);
	current = *sorted_list;
	temp = ft_strjoin(get_cwd(), "/./minishell");
	while (current)
	{
		key = get_key((char *)current->content);
		if (!ft_strncmp(key, "_=", 3) && ft_strncmp(find_value(key,
			call->env, 1) + 2, temp, ft_strlen(temp)))
			free(key);
		else
			write_env1(key, call, current);
		current = current->next;
	}
	ft_lstclear(sorted_list, &free);
	free(sorted_list);
	free(temp);
	return (EXIT_SUCCESS);
}

static int		check_export_errors(char *func)
{
	int			i;

	if (func[0] == '=')
	{
		// ft_printf_e("minishell: export: '%s': not a valid identifier\n", func);
		ft_printf_e("bash: line 1: export: ");
		ft_printf_e("`%c': not a valid identifier\n", func[0]);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (func[i] && func[i] != '=')
	{
		if (!ft_isalnum((int)func[i]) && func[i] != '_')
		{
			// ft_printf_e("minishell: export: '%s': not a valid identifier\n", func);
			ft_printf_e("bash: line 1: export: ");
			ft_printf_e("`%c': not a valid identifier\n", func[i]);
			return ((g_exit_nb = EXIT_FAILURE));
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static void		export_alone(char *str, t_call *call)
{
	if (str == NULL)
		ft_env1(call);
}

int			ft_export(t_call *call, char **func)
{
	int			i;
	char		*key;

	i = 0;
	export_alone(func[1], call);
	while (func[++i])
	{
		if (check_export_errors(func[i]))
			return (EXIT_FAILURE);
		if (func[i][0] != '=')
		{
			key = get_key(func[i]);
			if (find_value(key, call->env, 1))
			{
				if (ft_strchr(func[i], '='))
					add_env(call, key, ft_strchr(func[i], '=') + 1, 1);
			}
			else if (ft_strchr(func[i], '='))
				add_env(call, key, ft_strchr(func[i], '=') + 1, 0);
			else
				add_env(call, func[i], "\0", 0);
			free(key);
		}
	}
	return (EXIT_SUCCESS);
}
