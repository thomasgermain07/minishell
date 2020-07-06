/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/25 15:12:10 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/06 11:23:22 by thgermai         ###   ########.fr       */
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
		&& str[i] != ';' && str[i] != '\'' && str[i] != '"')
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

static char		*replace_var(char *str, int index, t_list **env)
{
	char		*temp;
	char		*var;
	char		*var_name;
	int			i;

	temp = ft_substr(str, 0, index);
	var_name = get_name_var(str + index);
	var = find_value(var_name, env);
	if (var)
		temp = ft_strjoin_f1(temp, var + ft_strlen(var_name) + 1);
	i = index;
	while (str[i] && str[i] != '"' && str[i] != '\'' &&
		str[i] != ' ' && str[i] != '|' && str[i] != ';')
		i++;
	temp = ft_strjoin_f1(temp, str + i);
	free(str);
	free(var_name);
	return (temp);
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
			str = replace_var(str, i, env);
			n--;
		}
		i++;
	}
	return (str);
}
