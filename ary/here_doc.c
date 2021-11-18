/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 09:48:07 by megen             #+#    #+#             */
/*   Updated: 2021/11/18 12:43:18 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/header.h"

static bool	here_doc_input(t_com *com, char *limiter, t_par *par)
{
	char	buffer[512];
	int		ret;

	write(1, "heredoc> ", 9);
	while (true)
	{
		ret = read(STDIN_FILENO, buffer, 511);
		if (ret < 0)
			return (false);
		if (!ret)
			return (true);
		buffer[ret - 1] = '\0';
		if (ary_strcmp(buffer, limiter))
			return (true);
		buffer[ret - 1] = '\n';
		buffer[ret] = '\0';
		write(com->p[1], buffer, ret);
		write(1, "heredoc> ", 9);
	}
}

static bool	here_doc_start(t_com *com, char *lim, t_par *par)
{
	int	pid;

	if (!wr_pipe(com, com->p))
		return (false);
	if (par->fd_hd)
		wr_close(com, par->fd_hd);
	par->fd_hd = com->p[0];
	pid = fork();
	if (pid < 0)
		return (err_set(par, error_fork));
	if (!pid)
	{
		wr_close(com, com->p[0]);
		here_doc_input(com, lim, par);
		wr_close(com, com->p[1]);
		exit(0);
	}
	else
	{
		wr_close(com, com->p[1]);
		waitpid(pid, NULL, 0);
	}
	return (true);
}

bool	here_doc(t_com *com)
{
	t_par	*par;
	t_snode	*node;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			par = par->next;
			node = node->next;
		}
		if (ary_strcmp("<<", node->value))
		{
			if (!here_doc_start(com, node->next->value, par))
				return (false);
			par->heredoc = true;
			node = node->next;
		}
		node = node->next;
	}
	return (true);
}
