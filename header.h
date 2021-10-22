/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:11:45 by megen             #+#    #+#             */
/*   Updated: 2021/10/22 14:46:30 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum e_error
{
	error_malloc = -1,
}t_error;

typedef struct s_snode
{
	void			*value;
	struct s_snode	*next;
	struct s_snode	*prev;
}t_snode;

typedef struct s_com
{
	int		prev_ret;
	t_snode	*arg_start;
	t_snode	*arg_last;
}t_com;

#endif
