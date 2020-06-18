/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/18 23:47:14 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			exec_binary(t_call	*call)
{
	char		**func;
	int			i;

	func = ft_split(call->str, ' '); // A remplacer par un parse d'arg : "", etc
	if (call->in != -1)
		dup2(call->in, 0);
	if (call->out != -1)
		dup2(call->out, 1);
	if (fork() == 0)
	{
		execvp(func[0], func); // A remplacer par un parse de function
		ft_printf_e("Error: execvp failed\n");
	}
	wait(NULL);
	i = -1;
	while (func[++i])
		free(func[i]);
	free(func);
}

void			parse_input(char *str)
{
	t_call		**calls;
	int			**pipes;
	int			i;

	i = -1;
	calls = check_pipes(str);
	while (calls[++i])
		parse_call(calls[i]);
	pipes = create_pipes(calls);
	/* if pipes branchez les pipes */
	/* exec_binary */
	/* clean pipes */
	clean_calls(calls);
	if (pipes)
		clean_pipes(pipes);
}

// void			prompt(void)
// {
// 	char *args;

// 	args = NULL;
// 	while (1)
// 	{
// 		ft_printf("MINISHELL -> ");
// 		get_next_line(0, &args);
// 		parse_input(args);
// 		free(args);
// 	}
// }

int			main(void)
{
//	prompt();
	char *str;

	str = ft_strdup("grep main < srcs/main.c  | echo test > txt.txt  | echo test > txt.txt  | echo test > txt.txt ");
	parse_input(str);
	printf("\n\nLeaving minishell\n\n");
	system("leaks minishell");
	return (0);
}
