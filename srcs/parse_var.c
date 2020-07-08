/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atetu <atetu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/25 15:12:10 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/08 18:38:12 by atetu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*get_name_var(char *str)
{
	int			i;

	i = 1;
	if (str[0] != '$')
		return (NULL);
	while (str[i] && str[i] != ' ' && str[i] != '|'
		&& str[i] != ';' && str[i] != '\'' && str[i] != '"' && str[i] != '=')
		i++;
	return (ft_substr(str, 1, i - 1));
}

static int		get_n_var(char *str)
{
	int			i;
	int			in_quote;
	int			count;

	count = 0;
	in_quote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' && !is_valide(str, i, 0))
			count++;
	}
	return (count);
}

static char		*replace_var(char *str, int index, t_list **env, int option)
{
	char		*temp;
	char		*var;
	char		*var_name;
	int			i;

	temp = ft_substr(str, 0, index);
	if (!option)
	{
		var_name = get_name_var(str + index);
		//printf("var name: %s\n", var_name);
		var = find_value(var_name, env);
	//	printf("var: %s\n", var);
		if (var)
			temp = ft_strjoin_f1(temp, var + ft_strlen(var_name) + 1);
		free(var_name);
	}
	if (option)
		temp = ft_strjoin_f12(temp, ft_itoa(exit_status));
	i = index;
	while (str[i] && str[i] != '"' && str[i] != '\'' &&
		str[i] != ' ' && str[i] != '|' && str[i] != ';' && str[i] != '=')
		i++;
	temp = ft_strjoin_f1(temp, str + i);
	free(str);
	return (temp);
}

static int odd_nb_backslash(char *str, int i)    ///ICI
{
	int count;

	count = 1;
	if (i == 0)
		return (1);
	while (i > 0)
	{
		if (str[i - 1] == '\\')
		{
			count++;
			i--;
		}
	}
	if (count % 2 == 0)
		return (0);
	else
		return (1);
}

char			*parse_var(char *str, t_list **env)
{
	int			i;
	int			n;

	n = get_n_var(str);
	if (!n)
		return (str);
	i = 0;
	while (n > 0 && str[i])
	{
		if (str[i] == '$' && !is_valide(str, i, 0))
		{
			if (i > 0 && str[i - 1] == '\\' && odd_nb_backslash(str, i - 1)) //ici
				;
			else if (str[i + 1] && str[i + 1] == '?') // marche pas encore
				str = replace_var(str, i, env, 1);
			else
				str = replace_var(str, i, env, 0);
			n--;
		}
		i++;
	}
	return (str);
}
