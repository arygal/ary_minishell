/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 16:54:32 by maearly           #+#    #+#             */
/*   Updated: 2021/10/21 16:54:34 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	NODE_H
#define NODE_H

# include "minishell.h"

enum e_node_type
{
	NODE_CMD,
	NODE_ARG,
};

typedef struct s_node
{
	enum e_node_type	type;
	enum e_val_type		val_type;
	char				*val;
	int					children;
	struct s_node		*first_child;
	struct s_node		*next_sibling;
	struct s_node		*prev_sibling;
}	t_node;

#endif
