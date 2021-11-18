/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniparser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 16:13:04 by maearly           #+#    #+#             */
/*   Updated: 2021/10/13 16:13:05 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROPARSER_H
#define MICROPARSER_H

#include "minishell.h"

typedef struct s_cmdLine
{
	char				**av;
	int					ac;
	char				*path;
	bool				isExec;
	bool				isBuiltin;
	int					fdIn;
	int					fdOut;
	int					fileMode;
	struct s_cmdLine	*next;
	struct s_cmdLine	*prev;
}	t_cmdLine;

t_cmdLine	*microParser(t_cmdLine *cmdLine, char *line);
void		runCommand(t_cmdLine *cmdLine);





#endif //ALPHA_MICROPARSER_H
