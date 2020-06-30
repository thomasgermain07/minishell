/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:53:30 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/30 20:52:01 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
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
	t_list			**env;
	int				ret;
}					t_call;

// exec_binary.c
pid_t				exec_binary(t_call	*call, int pipes[][2], int size);
void				exec_alone(t_call *call);
// parse_call.c
void				parse_call(t_call *call, t_list **env);
// check_n_pipes.c
int					get_n_pipes(char *args, int option);
void				check_pipes(char *str, t_call *calls);
void				create_pipes(t_call *calls, int pipes[][2]);
void				connect_pipes(t_call *calls, int pipes[][2]);
// utiles_func.c
void				close_pipes(int	pipes[][2], int size);
void				clean_calls(t_call *calls);
char				*get_cwd(void);
t_list				**tab_to_list(char **env);
char				**list_to_tab(t_list **lst);
void				clean_array(char **array);
char				*find_value(char *str, t_list **env);
int					known_func(char *str);
// parse_func.c
char				**parse_func(char *str);
// replace_var.c
char				*parse_var(char *str, t_list **env);
// builtin.c
int	 				ft_echo(char **func);
int 				ft_cd(char **func);
int					ft_pwd(void);
// builtin_env.c
int					ft_env(t_call *call);
int					ft_unset(t_call *call, char **func);
int					ft_export(t_call *call, char **func);
// main.c
void				wait_pids(pid_t *pids, int size, t_call *calls);
#endif

/*
	NOTES :

	- Parsing all env var has to be done before everything cause it
		can be used for redirection for exemple
	- for ' and " check if !n_quote for n_dquote++ etcpo
*/
