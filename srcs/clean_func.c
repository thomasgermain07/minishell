/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 23:43:21 by thgermai          #+#    #+#             */
/*   Updated: 2020/06/21 17:00:00 by thgermai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			clean_calls(t_call *calls)
{
	int			i;

	i = -1;
	while ((calls +  ++i)->str)
		free((calls + i)->str);
}

void			close_pipes(int pipes[][2], int size)
{
	int			i;

	i = -1;
	while (++i < size)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

/*

{
    "explorer.confirmDragAndDrop": false,
    "files.autoSave": "onFocusChange",
    "editor.tabSize": 4,
    "editor.insertSpaces": false,
    "editor.renderWhitespace": "none",
    "files.trimTrailingWhitespace": true,
    "files.insertFinalNewline": true,
    "editor.minimap.enabled": false,
    "explorer.confirmDelete": false,
    "window.zoomLevel": 0,
    "42header.email": "thgermai@student.42.fr",
    "42header.username": "thgermai",
    "workbench.startupEditor": "newUntitledFile"
}


*/
