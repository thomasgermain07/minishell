/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 11:30:16 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/25 15:00:19 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		count_args(char *str)
{
	int				count;
	int				i;
	int				in_quote;
	int				in_dquote;

	count = 0;
	i = -1;
	in_quote = 0;
	in_dquote = 0;
	while (str[++i] == ' ')
		;
	while (str[++i])
	{
		if (str[i] == '\'' && i > 0 && str[i - 1] != '\\')
			in_quote == 1 ? in_quote-- : in_quote++;
		if (str[i] == '"' && i > 0 && str[i - 1] != '\\')
			in_dquote == 1 ? in_dquote-- : in_dquote++;
		if (str[i] == ' ' && str[i + 1] && str[i + 1]
			!= ' ' && !in_dquote && !in_quote)
			count++;
	}
	return (count);
}

static int		create_line(char *str, char **tab)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	in_quote = 0;
	in_dquote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (i > 0 && str[i - 1] == '\\')
				;
			else if (str[i] == '"')
				in_dquote == 1 ? in_dquote-- : in_dquote++;
			else
				in_quote == 1 ? in_quote-- : in_quote++;
		}
		if ((str[i] == ' ' || !str[i + 1]) && !in_quote && !in_dquote)
		{
			!str[i + 1] && str[i] != ' ' ? i++: (void)i;
			return (ft_strlen((*tab = ft_substr(str, 0, i))));
		}
	}
	return (i);
}

char			**parse_func(char *str)
{
	int				size;
	int				i;
	char			**tab;

	i = -1;
	if (!str || !ft_strlen(str))
		return (NULL);
	size = count_args(str) + 1;
	if (!(tab = malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	while (++i < size)
	{
		while (*str == ' ')
			str++;
		str = str + create_line(str, &tab[i]);
	}
	tab[i] = NULL;
	return (tab);
}
