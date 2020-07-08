/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:58:27 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/08 11:03:58 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*replace_str(char *str)
{
	int			i;
	int			j;
	char		*temp;

	j = 0;
	i = -1;
	while (str[++i])
		if (str[i] == -1 || str[i] == -2)
			j++;
	if (!j)
		return (str);
	if (!(temp = malloc(sizeof(char) * (ft_strlen(str) - j + 1))))
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] != -1 && str[i] != -2)
			temp[j++] = str[i];
	temp[j] = '\0';
	free(str);
	return (temp);
}

char			*parse_quotes(char *str)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	i = -1;
	in_quote = 0;
	in_dquote = 0;
	while (str[++i])
	{
		if (str[i] == '"' &&
			(i == 0 || (i > 0 && str[i - 1] != -2)) && !in_quote)
		{
			in_dquote == 1 ? in_dquote-- : in_dquote++;
			str[i] = -1;
		}
		else if (str[i] == '\'' &&
			(i == 0 || (i > 0 && str[i - 1] != -2)) && !in_dquote)
		{
			in_quote == 1 ? in_quote-- : in_quote++;
			str[i] = -1;
		}
		else if (str[i] == '\\' &&
			(i == 0 || (i > 0 && str[i - 1] != -2)) && !in_quote)
			str[i] = -2;
	}
	return (replace_str(str));
}
