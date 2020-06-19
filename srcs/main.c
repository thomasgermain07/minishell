/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 10:44:15 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/19 11:05:40 by thgermai         ###   ########.fr       */
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
	t_call		calls[get_n_pipes(str, 0) + 2];
	int			pipes[get_n_pipes(str, 0)][2];
	int			i;

	i = -1;
	check_pipes(str, calls);
	while (calls[++i].str)
		parse_call(&calls[i]);
	for (int x = 0; calls[x].str; x++)
		printf("%s %d %d\n", calls[x].str, calls[x].in, calls[x].out);
	if (i > 0)
		create_pipes(calls, pipes);
	/* if pipes branchez les pipes */
	/* exec_binary */
	clean_calls(calls);
	if (pipes[0])
		clean_pipes(pipes, get_n_pipes(str, 0));
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

	str = ft_strdup("grep main < srcs/main.c  | echo test > txt.txt  | echo test | echo test > txt.txt ");
	parse_input(str);

	printf("\n\nLeaving minishell\n\n");
	system("leaks minishell");
	return (0);
}
