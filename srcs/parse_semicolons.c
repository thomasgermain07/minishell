/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_semicolons.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 15:24:14 by atetu             #+#    #+#             */
/*   Updated: 2020/08/17 14:49:18 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static int		get_n_semicolon(char *args, int option)
{
	int			i;
	int			n_semicolon;
	int			j; // ICI

	i = 0;
	n_semicolon = 0;
	while (args[++i])
	{
		if (args[i] == ';' && !is_valide(args, i, 1) && (i == 0 ||
		(i > 0 && !is_backslash(args, i - 1)))) //ICI)
		{
			if (option == 1)
				return (i);
			if (i > 0)   /// ICIC
			{
				j = i -1;
				while (j >=0 && args[j] == ' ')
					j--;
			}
			if (args[j] == '>' || args[j] == '<')  // ICI
			{
				g_exit_status = 258;  // a revoir???
				g_exit_nb = 2;  /// ICICIICICICI exit
				ft_printf_e("bash: line 1: syntax error near unexpected token ';'\n");  // JUSTE POUR LEs TESTS
			//	ft_printf_e("minihell: syntax error near unexpected token ';'\n");
				return (-1);
			}
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
	int			n_semicolons;

	last_i = 0;
	i = 0;
	j = 0;
	if ((n_semicolons = get_n_semicolon(str, 0)) == -1)   //ICIC
		return (NULL);
	if (!(tab = malloc(sizeof(char *) * (n_semicolons + 2))))
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
