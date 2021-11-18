/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:11:45 by megen             #+#    #+#             */
/*   Updated: 2021/10/21 12:58:27 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

#include "minishell.h"

typedef enum e_err
{
	error_malloc = -1,
}	t_err;

typedef struct s_snode
{
	void			*value;
	int				prev_ret;
	struct s_snode	*next;
	struct s_snode	*prev;
}	t_snode;

typedef struct s_com
{
	t_snode *arg_start;
	t_snode *arg_last;
	int 	prev_ret;
	int		ac;
	int 	fdIn;
	int 	fdOut;
	char 	**av;
}	t_com;

int		exec(t_com *com);
int		parser(t_com *com, char *line);
int		execBuiltin(t_com *cmd);
//void	handleErrors(t_com *cmd, bool ispath, int errno_);

void print_node(t_com *com);
void	runCmd(t_com *com);



#endif
