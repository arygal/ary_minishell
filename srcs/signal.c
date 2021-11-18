/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 20:56:45 by maearly           #+#    #+#             */
/*   Updated: 2021/10/11 20:56:47 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//void	signalHandlerParent(int sig)
//{
//	if (sig == SIGQUIT && g_conf.forked)
//	{
//		write(2, "Quit: 3\n", 8);
//		rl_on_new_line();
//	}
//	if (sig == SIGQUIT && !g_conf.forked)
//	{
//		rl_on_new_line();
//		rl_redisplay();
//	}
//	if (sig == SIGINT && g_conf.forked)
//	{
//		write(1, "\n", 1);
//		rl_on_new_line();
//	}
//	if (sig == SIGINT && !g_conf.forked)
//	{
//		write(1, "\n", 1);
//		rl_on_new_line();
//		rl_replace_line("", 1);
//		rl_redisplay();
//		updateCode(1);
//	}
//}
//
//void	updateCode(int code)
//{
//	char	*codeStr;
//
//	if (code >= 0)
//	{
//		codeStr = ft_itoa(code);
//		setVar2(g_conf.envp, "?", codeStr, false);
//	}
//	else
//	{
//		codeStr = ft_itoa(WEXITSTATUS(g_conf.status));
//		setVar2(g_conf.envp, "?", codeStr, false);
//	}
//	if (codeStr)
//		free(codeStr);
//}


void	interruptHereDoc(int signal)
{
	(void)signal;
	g_conf.com->prev_ret = 130;
	write(1, "\n", 1);
	exit(130);
}

void	interruptProcess(int signal)
{
	(void)signal;
	g_conf.com->prev_ret = 130;
	write(1, "\n", 1);
}

static void	redisplayPrompt(int signal)
{
//	printf("Hi from redisplayPrompt - 1\n");

	(void)signal;
	g_conf.com->prev_ret = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

//	printf("Hi from redisplayPrompt - 2\n");
}

static void	quitProcess(int signal)
{
	(void)signal;
	g_conf.com->prev_ret = 131;
	printf("Quit (core dumped)\n");
}

void	defineExecSignals(void)
{
	signal(SIGINT, interruptProcess);
	signal(SIGQUIT, quitProcess);
}

void	defineInputSignals(void)
{
//	printf("Hi from defineInputSignals\n");
	signal(SIGINT, redisplayPrompt);
	signal(SIGQUIT, SIG_IGN);
}
