/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 16:22:07 by maearly           #+#    #+#             */
/*   Updated: 2021/10/21 16:22:09 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

# include "minishell.h"

typedef struct s_lexer
{
	char	*input;
	int		position;
	int		read_position;
	char	ch;
	int		len;
}	t_lexer;


t_lexer	*newLexer(const char *line, int cmd_length);
void	nextChar(t_lexer *lexer);

#endif
