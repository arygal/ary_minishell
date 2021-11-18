/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_add.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 08:49:51 by megen             #+#    #+#             */
/*   Updated: 2021/11/18 12:40:38 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/header.h"

bool	wait_all_pids(t_com *com)
{
	t_pid_node	*node;

	while (com->pw_list.head)
	{
		node = com->pw_list.head;
		if (waitpid(node->pid, NULL, 0) == -1)
			return (false);
		if (WIFEXITED(node->pid))
			com->prev_ret = WIFEXITED(node->pid);
		com->pw_list.head = com->pw_list.head->next;
		free(node);
	}
	return (true);
}
