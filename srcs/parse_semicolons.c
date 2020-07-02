/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_semicolons.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 15:24:14 by atetu             #+#    #+#             */
/*   Updated: 2020/07/02 23:05:17 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		get_n_semicolon(char *args, int option)
{
	int			i;
	int			n_semicolon;

	i = 0;
	n_semicolon = 0;
	while (args[++i])
	{
		if (args[i] == ';' && !is_valide(args, i, 1))
		{
			if (option == 1)
				return (i);
			n_semicolon++;
		}
	}
	if (option == 1)
		return (-1);
	return (n_semicolon);
}

char			**parse_semicolon(char *str)
{
	int			i;
	int			j;
	int			last_i;
	char		**tab;

	last_i = 0;
	i = 0;
	j = 0;
	if (!(tab = malloc(sizeof(char *) * (get_n_semicolon(str, 0) + 2))))
		return (NULL);
	while (get_n_semicolon(str + last_i, 1) != -1)
	{
		tab[j] = ft_substr(str + last_i, 0,
			get_n_semicolon(str + last_i, 1));
		last_i += ft_strlen(tab[j]) + 1;
		i++;
		j++;
	}
	tab[j] = ft_substr(str + last_i, 0, ft_strlen(str + last_i));
	tab[j + 1] = NULL;
	return (tab);
}
