/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_call.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 16:15:55 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/22 14:21:38 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* option: 1 < | 2 > | 3 >> */
static int		get_fd(char *str, int option)
{
	char		*file_name;
	int			start;
	int			i;
	int			fd;

	i = 0;
	fd = 0;
	while (str[i] && str[i] == ' ')
		i++;
	start = i;
	while (str[i] && str[i] != ' ')
		i++;
	file_name = ft_substr(str, start, i - start);
	if (option == 1)
		fd = open(file_name, O_RDONLY);
	else if (option == 2)
		fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (option == 3)
		fd = open(file_name, O_RDWR | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		ft_printf_e("Error: %s: %s\n", file_name, strerror(errno));
	free(file_name);
	return (fd);
}

static int		check_input(t_call *call, int i)
{
	int			in_quote;

	in_quote = 0;
	while (call->str[++i])
	{
		if (call->str[i] == '"' && i > 0 && call->str[i - 1] != '\\')
			in_quote == 1 ? in_quote-- : in_quote++;
		if (call->str[i] == '<' && !in_quote)
		{
			if (call->in != -1)
				close(call->in);
			if ((call->in = get_fd(&call->str[i + 1], 1)) == -1)
				exit(5);
			check_input(call, i);
			return (1);
		}
	}
	return (0);
}

static int		check_output(t_call *call, int i)
{
	int			in_quote;

	in_quote = 0;
	while (call->str[++i])
	{
		if (call->str[i] == '"' && i > 0 && call->str[i - 1] != '\\')
			in_quote == 1 ? in_quote-- : in_quote++;
		if (call->str[i] == '>' && !in_quote)
		{
			if (call->out != -1)
				close(call->out);
			if (call->str[i + 1] == '>')
			{
				if ((call->out = get_fd(&call->str[i + 2], 3)) == -1)
					exit(7);
			}
			else if ((call->out = get_fd(&call->str[i + 1], 2)) == -1)
				exit(6);
			check_output(call, i);
			return (1);
		}
	}
	return (0);
}

static void		get_args(t_call *call)
{
	int			i;
	char		*str;
	int			in_quote;

	i = -1;
	str = NULL;
	in_quote = 0;
	while (call->str[++i])
	{
		if (call->str[i] == '"' && i > 0 && call->str[i - 1] != '\\')
			in_quote == 1 ? in_quote-- : in_quote++;
		if (call->str[i] == '>' || call->str[i] == '<')
		{
			str = ft_substr(call->str, 0, i);
			free(call->str);
			call->str = str;
			return ;
		}
	}
}

void			parse_call(t_call *call, char **env)
{
	call->env = env;
	call->in = -1;
	call->out = -1;
	if (!check_input(call, -1))
		call->in = 0;
	if (!check_output(call, -1))
		call->out = 1;
	get_args(call);
}
