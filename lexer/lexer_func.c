/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 16:27:48 by maearly           #+#    #+#             */
/*   Updated: 2021/10/21 16:27:49 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer.h"

void	nextChar(t_lexer *lexer)
{
	if (!lexer || !lexer->input)
		errno = ENODATA;
	if (lexer->read_position >= lexer->len)
		lexer->ch = 0;
	else
		lexer->ch = lexer->input[lexer->read_position];
	lexer->position = lexer->read_position;
	++lexer->read_position;
}

t_lexer	*newLexer(const char *line, int cmd_length)
{
	t_lexer	*lexer;

	lexer = ft_malloc(sizeof(t_lexer));
	*lexer = (t_lexer){.input = (char *) line, .len = cmd_length,
			.read_position = 0, .position = 0,
			.ch = ' '};
	nextChar(lexer);
	return (lexer);
}

