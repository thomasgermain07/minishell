/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:53:30 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/19 11:58:02 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
// temporary lib
# include <stdio.h>

typedef	struct		s_call
{
	char			*str;
	int				in;
	int				out;
}					t_call;

void				parse_call(t_call *call);
void				check_pipes(char *str, t_call *calls);
void				create_pipes(t_call *calls, int pipes[][2]);
void				connect_pipes(t_call *calls, int pipes[][2]);
void				clean_pipes(int	pipes[][2], int size);
void				clean_calls(t_call *calls);

int					get_n_pipes(char *args, int option);

#endif

/*
	NOTES :

	- ajouter char **env au main pour recup les variables d'env
	- check avec stat si le binaire existe
*/
