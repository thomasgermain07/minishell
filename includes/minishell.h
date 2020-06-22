/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:53:30 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/22 15:28:10 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
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
	char			**env;
}					t_call;

// exec_binary.c
void				exec_binary(t_call	*call, int pipes[][2], int size);
// parse_call.c
void				parse_call(t_call *call, char **env);
// check_n_pipes.c
int					get_n_pipes(char *args, int option);
void				check_pipes(char *str, t_call *calls);
void				create_pipes(t_call *calls, int pipes[][2]);
void				connect_pipes(t_call *calls, int pipes[][2]);
// clean_func.c
void				close_pipes(int	pipes[][2], int size);
void				clean_calls(t_call *calls);


#endif

/*
	NOTES :

	- ajouter char **env au main pour recup les variables d'env
	- check avec stat si le binaire existe
*/
