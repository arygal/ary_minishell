/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 16:13:48 by maearly           #+#    #+#             */
/*   Updated: 2021/10/12 16:13:51 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	action(char *line)
{
	int		code;


	code = parser_ary(g_conf.com, line);

//	print_node(&com);

//	if (com.ac > 0 && isBuiltin((char *)com.arg_start->value))
//		return (execBuiltin(&com));
//	else if (com.ac > 0)
//		runCmd(&com);
	return (0);
}

char	*readLine(void)
{
	char	*line;

	defineInputSignals();
//	line = readline(g_conf.prompt);
	line = readline("minishell $ ");
	rl_replace_line("", 0);
	if (line && *line)
		add_history (line);
	return (line);
}

//	TODO:	free(cmdLine);
void	mainLoop(char **env)
{
//	t_cmdLine	*cmdLine;
	char		*line;
	int			code;
	t_com	com;

	g_conf.com = &com;


//	cmdLine = malloc(sizeof(t_cmdLine));
//	if (!cmdLine)
//		exit (1);
	while (true)
	{
		line = readLine();
		if (line && line[0] == '\0')
		{
			free(line);
			continue ;
		}
		if (!line)
		{
			if (isatty(0))
				write(2, "exit\n", 5);
			break ;
		}
//		cmdLine = microParser(cmdLine, line);
//		runCommand(cmdLine);
		action(line);
	}
}
