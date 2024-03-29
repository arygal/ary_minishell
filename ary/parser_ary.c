/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:11:45 by megen             #+#    #+#             */
/*   Updated: 2021/11/18 22:36:32 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	countArgc(t_com *com)
{
	t_snode	*head;
	int		ac;

	head = com->arg_start;
	ac = 0;
	while (head)
	{
		++ac;
		if (head)
			head = head->next;
	}
	return (ac);
}

char	**listToArgv(t_com *com)
{
	char	**tmp;
	int		i;
	t_snode	*head;

	tmp = ft_malloc(sizeof(char *) * (com->ac + 1));
	head = com->arg_start;
	i = -1;
	while (++i < com->ac)
	{
		tmp[i] = (char *)head->value;
		head = head->next;
	}
	tmp[i] = NULL;
	return (tmp);
}

int	count_args(char **av)
{
	int	count;

	count = 0;
	while (av[count])
		count++;
	return (count);
}

void	com_init(t_com *com)
{
	com->arg_start = NULL;
	com->pw_list.head = NULL;
	com->inh_list.head = NULL;
	com->par_head = NULL;
	com->syntax = 0;
}

int	parser_ary(t_com *com, char *line)
{
	com_init(com);
	com->envp = extractEnvp(g_conf.envp);
	com->ac = countArgc(com);
	com->av = listToArgv(com);
//	printf("11444\n");
	if (!line || !*line)
		return (0);
	if (empty_line(line))
		return(0);
	if (!syntax_err(com, line))
//		return (write(1, "SINTAX\n", 7));
		return (syntaxError("syntax error near unexpected token ",
						NULL, line, 258));
	/*SYNTAX ERROR HERE => TOKEN IN COM->SYNTAX*/
	if (!line_to_par(com, line))
		handleErrors(NULL, false, error_malloc);
	if (!split_line_to_pars(com, line))
		handleErrors(NULL, false, error_malloc);
	if (!par_envp(com))
		handleErrors(NULL, false, error_malloc);
	free(line);
	par_to_parse(com);
	if (!reform_nodes(com) && com->term)
	{
		free_all(com, com->par_head);
		return(0);
	}
	execute_pipeline(com, com->par_head); // зануляет $? heredoc

	wait_all_pids(com);
	close_inhereted(com, 0, 1);

	free_all(com, com->par_head);

	return (0);
}
