/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 10:54:54 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/18 11:06:25 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*get_file_name(char *str)
{
	int		i;

	i = 0;
	while (str[i] && str[i] != ' ')
		i++;
	return(ft_substr(str, 0, i));
}

static void		check_for_input(t_call *call)
{
	char		*str;
	int			i;
	int			in_quote;

	i = -1;
	in_quote = 0;
	str = NULL;
	while (call->str[++i])
	{
		if (call->str[i] == '"' && i > 0 && call->str[i - 1]!= '\\')
			in_quote == 1 ? in_quote-- : in_quote++ ;
		if (call->str[i] == '<' && in_quote == 0)
		{
			while (call->str[++i] == ' ')
				;
			if ((call->in = open((str = get_file_name(&(call->str[i]))), O_RDONLY)) == -1)
				exit(5);
			free(str);
		}
	}
}

static void		check_for_output(t_call *call)
{
	char		*str;
	int			i;
	int			in_quote;

	i = -1;
	in_quote = 0;
	str = NULL;
	while (call->str[++i])
	{
		if (call->str[i] == '"' && i > 0 && call->str[i - 1]!= '\\')
			in_quote == 1 ? in_quote-- : in_quote++ ;
		if (call->str[i] == '>' && in_quote == 0)
		{
			if (call->str[i + 1] == '>' && (i += 1))
			{
				while (call->str[++i] == ' ')
				;
				if ((call->out = open((str = get_file_name(&(call->str[i]))), O_RDWR | O_APPEND | O_CREAT)) == -1)
					exit(6);
				free(str);
			}
			else
			{
				while (call->str[++i] == ' ')
					;
				if ((call->out = open((str = get_file_name(&(call->str[i]))), O_RDWR | O_CREAT)) == -1)
					exit(7);
				free(str);
			}
		}
	}
}

void			configure_calls(t_call *calls)
{
	int			i;
	int			j;
	int			in_quote;
	char		*str;

	i = -1;
	str = NULL;
	in_quote = 0;
	while ((calls + ++i)->str)
	{
		j = -1;
		check_for_input((calls + i));
		check_for_output((calls + i));
		str = ft_strdup((calls + i)->str);
		free((calls + i)->str);
		while (str[++j])
		{
			if (str[j] == '"' && i > 0 && str[j - 1]!= '\\')
				in_quote == 1 ? in_quote-- : in_quote++ ;
			if (str[j] == '<' || str[j] == '>')
				(calls + i)->str = ft_substr(str, 0, j);
		}
		free(str);
	}
}
