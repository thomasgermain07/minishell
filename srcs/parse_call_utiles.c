/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_call_utiles.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:55:52 by atetu             #+#    #+#             */
/*   Updated: 2020/08/28 14:49:14 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		get_fd(char *str, int option)
{
	char		*file_name;
	int			start;
	int			i;
	int			fd;

	i = 0;
	fd = 0;
	while (str[i] && str[i] == ' ')
		i++;
	start = i;
	while (str[i] && str[i] != ' ')
		i++;
	file_name = ft_substr(str, start, i - start);
	if (option == 1)
		fd = open(file_name, O_RDONLY);
	else if (option == 2)
		fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (option == 3)
		fd = open(file_name, O_RDWR | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		ft_printf_e("minishell: %s: %s\n", file_name, strerror(errno));
	free(file_name);
	return (fd);
}