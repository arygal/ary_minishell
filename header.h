/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:11:45 by megen             #+#    #+#             */
/*   Updated: 2021/11/06 17:42:31 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>

typedef enum e_error
{
	error_malloc = -6,
	error_syntax,
	error_nofile,
	error_noprem,
	error_nocom,
	error_create,
	error_open,
}t_error;

typedef struct s_inh_node
{
	int					fd;
	struct s_inh_node	*next;
}t_inh_node;

typedef struct s_inh_list
{
	t_inh_node			*head;
	t_inh_node			*tail;
}t_inh_list;

typedef struct s_snode
{
	void			*value;
	struct s_snode	*next;
	struct s_snode	*prev;
}t_snode;

typedef struct s_pid_node
{
	int					pid;
	struct s_pid_node	*next;
}t_pid_node;


typedef struct s_pw_list
{
	t_pid_node	*head;
	t_pid_node	*tail;
}s_pw_list;


typedef struct s_argv_list
{
	int			size;
	t_snode		*head;
	t_snode		*tail;
}t_argv_list;

typedef struct s_par
{
	int				fd_out;
	int				fd_in;
	int				error;
	bool			append;
	bool			heredoc;
	bool			input_err;
	char			*path;
	char			**argv;
	char			**envp;
	t_snode			*head;
	t_snode			*tail;
	t_snode			*error_node;
	t_snode			*out_node;
	t_snode			*in_node;
	t_snode			*exe;
	struct s_par	*next;	
}t_par;

typedef struct s_com
{
	int			prev_ret;
	char		**envp;
	t_snode		*arg_start;
	t_snode		*arg_last;
	t_par		*par_head;
	t_par		*par_tail;
	t_inh_list	inh_list;
	s_pw_list	pw_list;
}t_com;

/*		utils.c */

int		ary_strlen(char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ary_strdup(char *src);
bool	ary_strcmp(char *src, char *trgt);

/*		utils_add.c */ 

char	**ft_split(char const *s, char c);

/*		wrapper.c */

int		wr_open(t_com *cmd, char *name, int mode);
bool	wr_pipe(t_com *cmd, int *p);
bool	wr_close(t_com *cmd, int fd);

/*		inhereted.c */

void	close_inhereted(t_com *cmd, int in, int out);
bool	add_to_inh_list(t_inh_list *list, int fd);


#endif
