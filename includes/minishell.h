/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:53:30 by thgermai          #+#    #+#             */
/*   Updated: 2020/07/07 11:14:03 by thgermai         ###   ########.fr       */
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
# include <signal.h>

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

// variables globales
pid_t				*g_pids;  //ALICE
int					exit_status;

///////////////////////////////////////////////////////////////

// Prompt.c
void				wait_pids(pid_t *pids, int size, t_call *calls);
void				prompt(char **env);

//prompt_utiles.c // ici
void				print(void);

// Parse_semicolons.c
char				**parse_semicolon(char *str);

// Parse_pipes.c
void				parse_pipes(char *str, t_call *calls);
int					get_n_pipes(char *args, int option);

// Parse_call.c
void				parse_call(t_call *call, t_list **env);

// Parse_func.c
char				**parse_func(char *str, t_list **env);

// Parse_var.c
char				*parse_var(char *str, t_list **env);

// Parse_quotes.c
char				*parse_quotes(char *str);

// Parse_exec.c
char				*parse_exec(t_call *call, char *bin);

// Execute.c
pid_t				exec1(t_call *call, int pipes[][2], int size,
						int *exit_info); //ici
void				exec2(t_call *call, int *exit_info); //ici

// Execute_utiles.c
void				duplicate_fd(t_call *call);
int					execute(t_call *call, char **func, char **env, int *exit_info); //ici
void				exec_knonw(t_call *call, char **func, char **var_env, int *exit_info); //ici

// Builtin.c
int	 				ft_echo(char **func);
int 				ft_cd(char **func);
int					ft_pwd(void);
int					ft_builtin_exit(int *exit_info); // ici

// Builtin_env.c
int					ft_env(t_call *call);
int					ft_unset(t_call *call, char **func);
int					ft_export(t_call *call, char **func);

// Builtin_env_utiles.c
char				*get_key(char *str);
int					add_env(t_call *call, char *key, char *value, int option);
int					delete_element(t_call *call, char *key);

// Handle_pipes.c
void				create_pipes(t_call *calls, int pipes[][2]);
void				connect_pipes(t_call *calls, int pipes[][2]);

// Utiles_convert.c
t_list				**tab_to_list(char **env);
char				**list_to_tab(t_list **lst);

// Utiles.c
char				*get_cwd(void);
int					is_valide(char *str, int index, int option);
char				*find_value(char *str, t_list **env);
int					known_func(char *str);

// Clean.c
void				close_pipes(int	pipes[][2], int size);
void				clean_calls(t_call *calls);
void				clean_array(char **array);

// Signal.c
void				control_c(int sig);
int					control_d(void);
void				control_quit(int sig);

#endif

/*
	NOTES :

	- Parsing all env var has to be done before everything cause it
		can be used for redirection for exemple
	- for ' and " check if !n_quote for n_dquote++ etcpo
*/

/*
	PROBLEMES:

	echo espace quand $var pas bon
	message erreur en double quand fonction pas connu
*/
