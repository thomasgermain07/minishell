/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 14:12:58 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 15:18:56 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				is_valide(char *str, int index)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	i = 0;
	in_quote = 0;
	in_dquote = 0;
	while (str[i] && i < index)
	{
		if (str[i] == '"' && (i == 0 || (i > 0 && (str[i - 1] != '\\'))) && !in_quote)
			in_dquote == 1 ? in_dquote-- : in_dquote++;
		else if (str[i] == '\'' && (i == 0 || (i > 0 && (str[i - 1] != '\\'))) && !in_dquote)
			in_quote == 1 ? in_quote-- : in_quote++;
		i++;
	}
	if (str[i] && !in_quote)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static char		*replace_str(char *str)
{
	int			i;
	int			j;
	char		*temp;

	j = 0;
	i = -1;
	while (str[++i])
		if (str[i] == -1)
			j++;
	if (!j)
		return (str);
	if (!(temp = malloc(sizeof(char) * (ft_strlen(str) - j + 1))))
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] != -1)
			temp[j++] = str[i];
	temp[j] = '\0';
	free(str);
	return (temp);
}

char			*replace_quotes(char *str)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	i = -1;
	in_quote = 0;
	in_dquote = 0;
	while (str[++i])
	{
		if (str[i] == '"'  && (i == 0 || (i > 0 && (str[i - 1] != '\\'))) && !in_quote)
		{
			in_dquote == 1 ? in_dquote-- : in_dquote++;
			str[i] = -1;
		}
		else if (str[i] == '\''  && (i == 0 || (i > 0 && (str[i - 1] != '\\'))) && !in_dquote)
		{
			in_quote == 1 ? in_quote-- : in_quote++;
			str[i] = -1;
		}
	}
	return (replace_str(str));
}
