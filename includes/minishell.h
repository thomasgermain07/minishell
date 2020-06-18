/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:53:30 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/18 23:44:21 by thgermai         ###   ########.fr       */
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
void				exec_binary(t_call	*call);
t_call				**check_pipes(char *str);
int					**create_pipes(t_call **calls);

void				clean_pipes(int	**pipes);
void				clean_calls(t_call **calls);
// int					get_n_pipes(char *args, int option);

#endif

/*
	NOTES :

	- ajouter char **env au main pour recup les variables d'env
	- check avec stat si le binaire existe
*/
