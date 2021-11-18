/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 17:53:33 by maearly           #+#    #+#             */
/*   Updated: 2021/10/22 17:53:35 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	checker(t_com *com, char **envp, t_vect *pathes)
{
	printf("Hi from checker!\n");

	char *str;
	int i = -1;
//	while (envp[++i])
//		printf("envp[%d] = %s\n", i, envp[i]);
//	printf("\n");
//	printf("com->av[0] = %s\n", com->av[0]);
	printf("pathes->size = %d\n", pathes->size);
	str = ft_strdup(getVar(g_conf.envp, "PATH"));
	printf("PATH = %s\n", str);
	i = -1;
	while (pathes->data[++i])
		printf("path[%d] = %s\n", i, (char *)pathes->data[i]);


	printf("Checker done!\n");
}

//static void    left_pipe(t_cmd *cmd, char **envp)
//{
//	cmd->pid_one = fork();
//	if (cmd->pid_one < 0)
//		error_exit(cmd, error);
//	if (!cmd->pid_one)
//	{
//		cmd->fd1 = open(cmd->argv[1], O_RDONLY);
//		if (cmd->fd1 < 0)
//			error_exit(cmd, error_file_one);
//		if (dup2(cmd->fd1, STDIN_FILENO) == -1)
//			error_exit(cmd, error);
//		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
//			error_exit(cmd, error);
//		close(cmd->pipe[1]);
//		close(cmd->pipe[0]);
//		close(cmd->fd1);
//		if (execve(cmd->path_one, cmd->arg_one, envp))
//			error_exit(cmd, error_prog_one);
//	}
//}

int	execCmd(t_com *com)
{
	char	**envp;
	t_vect	*pathes;
	int 	i;

	envp = extractEnvp(g_conf.envp);
	pathes = getPathes(getVar(g_conf.envp, "PATH"), com->av[0]);

//	checker(com, envp, pathes);

	i = -1;
	while (++i < (int)pathes->size)
	{
		execve((char *)at(pathes, i), com->av, envp);
		if (errno != 2)
			break ;
	}
	deleteFree(pathes, &free);
//	handleErrors(com, false, errno);
	free(envp);
	return (1);
}

pid_t	runCmdChild(t_com *com)
{
	pid_t	pid;

	pid = fork();
	defineExecSignals();
	if (pid < 0)
		exit(1);
	if (pid == 0)
		exit(execCmd(com));
	return (pid);
}

void	runCmd(t_com *com)
{
	pid_t	pid;

	pid = runCmdChild(com);
	if (pid > 0)
		waitpid(pid, &g_conf.status, 0);
//	updateCode(-1);
	g_conf.com->prev_ret = -1;
}
