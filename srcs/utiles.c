/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 23:43:21 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/03 15:50:46 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char			*get_cwd(void)
{
	char		buffer[512];
	char		*cwd;

	ft_bzero(buffer, 512);
	cwd = getcwd(buffer, 512);
	return (cwd);
}

char			*find_value(char *str, t_list **env)
{
	t_list		*current;

	current = *env;
	while (current)
	{
		if (str[ft_strlen(str) - 1] == '=')
		{
			if (!ft_strncmp(str, (char *)current->content, ft_strlen(str)))
				return ((char *)current->content);
		}
		else
		{
			if (!ft_strncmp(str, (char *)current->content, ft_strlen(str)) &&
				((char *)current->content)[ft_strlen(str)] == '=')
				return ((char *)current->content);
		}
		current = current->next;
	}
	return (NULL);
}

int				known_func(char *str)
{//ATTENTION J'CHANGE TOUS LES NOMBRES
	if (!ft_strncmp(str, "echo", 5))
		return (1);
	else if (!ft_strncmp(str, "cd", 3))
		return (1);
	else if (!ft_strncmp(str, "pwd", 4))
		return (1);
	else if (!ft_strncmp(str, "export", 7))
		return (1);
	else if (!ft_strncmp(str, "unset", 6))
		return (1);
	else if (!ft_strncmp(str, "env", 4))
		return (1);
	else if (!ft_strncmp(str, "exit", 5)) // ici
		return (1); //ici
	return (0);
}

int				is_valide(char *str, int index, int option)
{
	int			i;
	int			in_quote;
	int			in_dquote;

	i = 0;
	in_quote = 0;
	in_dquote = 0;
	while (str[i] && i < index)
	{
		if (str[i] == '"' && (i == 0 || (i > 0 && (str[i - 1] != '\\')))
			&& !in_quote)
			in_dquote == 1 ? in_dquote-- : in_dquote++;
		else if (str[i] == '\'' && (i == 0 || (i > 0 && (str[i - 1] != '\\')))
			&& !in_dquote)
			in_quote == 1 ? in_quote-- : in_quote++;
		i++;
	}
	if (!option && str[i] && !in_quote)
		return (EXIT_SUCCESS);
	if (option && !in_quote && !in_dquote)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
