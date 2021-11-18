/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 16:53:01 by maearly           #+#    #+#             */
/*   Updated: 2021/10/11 16:53:03 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_shell	g_conf;



void	myLittleChecker()
{
	printf("%s\n", g_conf.prompt);

	listVars(g_conf.envp, false, printVar2, 0);

//	printf("capacity = %d\n", g_conf.envp->capacity);
//	printf("size = %d\n", g_conf.envp->size);
}

int	main(int ac, char **av, char **env)
{
	int		code;

	code = 0;
	initializate(ac, av, env);

	mainLoop(env);
//	myLittleChecker();
	return (EXIT_SUCCESS);
}
