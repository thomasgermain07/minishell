/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_semicolons.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/01 15:24:14 by atetu             #+#    #+#             */
/*   Updated: 2020/09/02 15:11:26 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int		syntax_error(void)
{
	g_exit_status = 2;
	g_exit_nb = 2;
	ft_printf_e("minihell: syntax error near unexpected token ';'\n");
	return (-1);
}

static int		result_semicolon(int option, int n_semicolon)
{
	if (option == 1)
		return (-1);
	return (n_semicolon);
}

static int		get_n_semicolon(char *args, int option)
{
	int			i;
	int			n_semicolon;
	int			j;

	i = -1;
	n_semicolon = 0;
	while (args[++i])
	{
		if (args[i] == ';' && !is_valide(args, i, 1) && (i == 0 ||
		(i > 0 && !is_backslash(args, i - 1))))
		{
			if (option == 1)
				return (i);
			if (i > 0)
			{
				j = i - 1;
				while (j >= 0 && args[j] == ' ')
					j--;
			}
			if (args[j] == '>' || args[j] == '<')
				return (syntax_error());
			n_semicolon++;
		}
	}
	return (result_semicolon(option, n_semicolon));
}

static int		arg_is_valid(char *str)
{
	int			i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (1);
	return (0);
}

static char		**handle_error_arg(int i, int n_semicolons, char **tab)
{
	if (i < n_semicolons)
	{
		i = -1;
		while (tab[++i])
			free(tab[i]);
		free(tab);
		return (NULL);
	}
	return (tab);
}

static char		**check_args(char **tab, int n_semicolons)
{
	int			i;

	i = -1;
	while (tab[++i + 1])
	{
		if (tab[i + 1] && !ft_strlen(tab[i + 1]) && i < n_semicolons - 1)
		{
			ft_printf_e("bash: syntax error near unexpected token `;;'\n");
			break ;
		}
		else if (!ft_strlen(tab[i]) || !arg_is_valid(tab[i]))
		{
			ft_printf_e("bash: syntax error near unexpected token `;'\n");
			break ;
		}
	}
	return (handle_error_arg(i, n_semicolons, tab));
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
	if ((n_semicolons = get_n_semicolon(str, 0)) == -1)
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
	// for (int x = 0; tab[x]; x++)
	// 	ft_printf("tab[%d] = %s<-\n", x, tab[x]);
	tab = check_args(tab, n_semicolons);
	return (tab);
}
