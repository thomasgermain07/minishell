/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles_convert.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 22:40:46 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/02 22:41:39 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char			**list_to_tab(t_list **lst)
{
	char		**tab;
	int			i;
	int			size;
	t_list		*current;

	size = ft_lstsize(*lst);
	i = -1;
	if (!(tab = malloc(sizeof(char *) * (size + 1))))
	{
		ft_printf_e("Minishell: error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	current = *lst;
	while (++i < size)
	{
		tab[i] = (char *)current->content;
		current = current->next;
	}
	tab[i] = NULL;
	return (tab);
}

t_list			**tab_to_list(char **env)
{
	int			i;
	t_list		**list;

	i = -1;
	if (!(list = malloc(sizeof(t_list *))))
	{
		ft_printf_e("Minishell: error: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	*list = NULL;
	while (env[++i])
		ft_lstadd_back(list, ft_lstnew(ft_strdup(env[i])));
	return (list);
}
