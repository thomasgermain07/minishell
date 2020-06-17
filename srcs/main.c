/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/17 14:44:17 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			exec_binary(char *str)
{
	char		**tab;
	int			i;

	i = -1;
	tab = ft_split(str, ' ');
	if (!fork())
		execvp(tab[0], tab);
	else
		wait(NULL);
	while (tab[++i])
		free(tab[i]);
	free(tab);
	exit(0);
}

void		prompt(void)
{
	char *args;

	args = NULL;
	while (1)
	{
		ft_printf("MINISHELL -> ");
		get_next_line(0, &args);
		check_pipes(args);
		free(args);
		system("leaks minishell");
	}
}

int			main(void)
{
	prompt();
	return (0);
}
