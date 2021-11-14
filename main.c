#include "header.h"

bool error_proc(int error, char *name)
{
	(void)name;
	printf("Err: %d\n", error);
	if (name)
		printf("Name: %s\n", name);
	exit(-1);
	return (false);
}

bool err_set(t_par *par, int error)
{
	par->error = error;
	return (false);
}

char *trim_space(char *line)
{
	while (*line == ' ' || *line == '	')
		++line;
	return (line);
}

char *quote(char *line, int *size)
{
	char *base;
	char spec;
	int len;

	spec = *line;
	++line;
	base = line;
	len = 0;
	while (*line)
	{
		if (*line == spec)
		{
			(*size) += len;
			return (++line);
		}
		++line;
		++len;
	}
	++(*size);
	return (base);
}

int get_size(char *line)
{
	int size;

	if ((line[0] == '<' && line[1] == '<') || (line[0] == '>' && line[1] == '>'))
		return (2);
	if (*line == '|' || *line == '<' || *line == '>')
		return (1);
	size = 0;
	while (*line)
	{
		if (*line == ' ')
			break;
		if (*line == '|' || *line == '<' || *line == '>')
			break;
		if (*line == '\'' || *line == '\"')
		{
			if (line[1] == *line)
				return (-1);
			line = quote(line, &size);
			continue;
		}
		++line;
		++size;
	}
	return (size);
}

bool find_next_spec(char *line, char spec)
{
	while (*line)
	{
		if (*line == spec)
			return (true);
		++line;
	}
	return (false);
}

char *fill_quote(char *line, char **newline, int *ct, char spec)
{
	if (!find_next_spec(line + 1, spec))
	{
		(*newline)[*ct] = *line;
		++(*ct);
		return (++line);
	}
	++line;
	while (*line != spec)
	{
		(*newline)[*ct] = *line;
		++(*ct);
		++line;
	}
	--(*ct);
	return (++line);
}

bool add_snode(t_com *com, char *line)
{
	t_snode *newnode;

	newnode = malloc(sizeof(t_snode));
	if (!newnode)
	{
		free(line);
		return (false);
	}
	newnode->value = line;
	if (!com->arg_start)
	{
		newnode->next = NULL;
		newnode->prev = NULL;
		com->arg_start = newnode;
		com->arg_last = newnode;
		return (true);
	}
	newnode->prev = com->arg_last;
	com->arg_last->next = newnode;
	newnode->next = NULL;
	com->arg_last = newnode;
	return (true);
}

char *add_arg(t_com *com, char *line, int size)
{
	char *newline;
	int ct;

	ct = -1;
	newline = malloc(sizeof(char) * (size + 1));
	if (!newline)
		return (NULL);
	while (++ct < size)
	{
		if (*line == '\'' || *line == '\"')
		{
			line = fill_quote(line, &newline, &ct, *line);
			continue;
		}
		newline[ct] = *line;
		++line;
	}
	newline[ct] = '\0';
	if (!add_snode(com, newline))
		return (NULL);
	return (line);
}

bool parse_line(t_com *com, char *line)
{
	int size;
	char *newline;

	line = trim_space(line);
	while (*line)
	{
		size = get_size(line);
		if (size == -1)
		{
			line += 2;
			newline = ary_strdup("");
			if (!newline)
				return (false);
			if (!add_snode(com, newline))
				return (false);
			line = trim_space(line);
			continue;
		}
		if (!size)
			return (true);
		line = add_arg(com, line, size);
		if (!line)
			return (false);
		line = trim_space(line);
	}
	return (true);
}

void free_snode(t_snode *head)
{
	t_snode *temp;

	while (head)
	{
		temp = head;
		free(temp->value);
		head = head->next;
		free(temp);
	}
}

void print_node(t_com *com)
{
	t_snode *temp;

	temp = com->arg_start;
	while (temp)
	{
		if (!temp->value)
			printf("NULL\n");
		else
			printf("%s\n", (char *)temp->value);
		temp = temp->next;
	}
}

char *quote_skip(char *line)
{
	char spec;
	char *base;

	spec = *line;
	++line;
	base = line;
	while (*line)
	{
		if (*line == spec)
			return (++line);
		++line;
	}
	return (base);
}

bool chek_line(char s)
{
	if (s == '-' || s == '?' || s == '*' || s == '$' || s == '@' || s == '_' || s == '#' ||
		s == '!' || (s >= '0' && s <= '9') || (s > 64 && s < 91) || (s > 96 && s < 123))
		return (true);
	return (false);
}

bool chek_back(char *line, char *base)
{
	if (line == base)
		return (true);
	--line;
	while (line != base)
	{
		if (*line == ' ' || *line == '	')
		{
			--line;
			continue;
		}
		if (*line != '<')
			return (true);
		else
		{
			--line;
			if (*line == '<')
				return (false);
			else
				return (true);
		}
	}
	return (true);
}

int count_envp(char *line)
{
	int ret;
	char *base;

	base = line;
	ret = 0;
	while (*line)
	{
		if (*line == '\'')
		{
			line = quote_skip(line);
			continue;
		}
		if (*line == '$' && chek_back(line, base) && chek_line(line[1]))
			++ret;
		++line;
	}
	return (ret);
}

int alt_quote_skip(char *line)
{
	int ct;
	char spec;

	ct = -1;
	spec = *line;
	++line;
	if (*line == spec)
		return (1);
	while (line[++ct])
		if (line[ct] == spec)
			return (ct + 2);
	return (1);
}

int size_of_envp(char *line)
{
	int ct;

	ct = -1;
	if (*line == '-' || *line == '?' || *line == '*' || *line == '$' || *line == '@' || *line == '_' || *line == '#' || *line == '!' || (*line >= '0' && *line <= '9'))
		return (1);
	while (line[++ct])
		if (!(line[ct] > 64 && line[ct] < 91) && !(line[ct] > 96 && line[ct] < 123))
			return (ct);
	return (ct);
}

void fill_env(char *line, char *env, int size)
{
	int ct;

	ct = -1;
	while (++ct < size)
		env[ct] = line[ct];
	env[ct] = '\0';
}

void chek_ambig(char *line, int place, t_par *par)
{
	if (place == 0)
		return;
	--place;
	while (place > 0)
	{
		if (line[place] == ' ' || line[place] == '	')
		{
			--place;
			continue;
		}
		else
			break;
	}
	if (line[place] == '<' && (place - 1) != 0 && line[place - 1] != '<')
		par->error = error_ambig;
	else if (line[place] == '>')
		par->error = error_ambig;
}

bool add_ambig_node(char *line, int place, int size, t_par *par)
{
	char *newline;
	t_snode *newnode;
	int ct;

	newline = malloc(sizeof(char) * (size + 1));
	if (!newline)
		return (false);
	ct = -1;
	while (++ct < size)
		newline[ct] = line[place + ct];
	newline[ct] = '\0';
	newnode = malloc(sizeof(t_snode));
	if (!newnode)
	{
		free(newline);
		return (false);
	}
	newnode->value = newline;
	par->error_node = newnode;
	return (true);
}

char *remove_env(char *line, int place, int size, t_par *par)
{
	int len;
	char *newline;
	int ct;

	chek_ambig(line, place, par);
	if (par->error == error_ambig)
	{
		if (!add_ambig_node(line, place, size, par))
			return (NULL);
		return (line);
	}
	len = ary_strlen(line) - size;
	newline = malloc(sizeof(char) * (len + 1));
	if (!newline)
		return (NULL);
	ct = -1;
	while (++ct < place)
		newline[ct] = line[ct];
	while (ct < len)
	{
		newline[ct] = line[size + ct];
		++ct;
	}
	newline[ct] = '\0';
	free(line);
	return (newline);
}

char *rewrite_env(char *line, int place, char *env_val, int size)
{
	int len;
	char *newline;
	int ct;
	int val_len;
	int i;

	val_len = ary_strlen(env_val);
	len = ary_strlen(line);
	newline = malloc(sizeof(char) * (len + val_len - size + 1));
	if (!newline)
		return (NULL);
	ct = -1;
	while (++ct < place)
		newline[ct] = line[ct];
	i = -1;
	while (++i < val_len)
		newline[ct++] = env_val[i];
	i = place + size;
	while (i < len)
		newline[ct++] = line[i++];
	newline[ct] = '\0';
	free(line);
	return (newline);
}

static char *ft_itoa_charset(char *ret, long int nclone, int size, int spec)
{
	while (size >= 0)
	{
		ret[size--] = nclone % 10 + 48;
		nclone = nclone / 10;
	}
	if (spec == 1)
		ret[0] = 45;
	return (ret);
}

char *ft_itoa(int n)
{
	long int nclone;
	int size;
	char *ret;

	nclone = n;
	size = 0;
	while (nclone != 0)
	{
		nclone = nclone / 10;
		size++;
	}
	if (n <= 0)
		size++;
	ret = malloc((size + 1) * sizeof(char));
	if (!ret)
		return ((void *)0);
	ret[size] = '\0';
	nclone = n;
	if (nclone < 0)
	{
		nclone = nclone * -1;
		return (ft_itoa_charset(ret, nclone, size - 1, 1));
	}
	else
		return (ft_itoa_charset(ret, nclone, size - 1, 0));
}

char *replace_envp(char *line, int place, int prev_ret, t_par *par)
{
	int size;
	char *env;
	char *env_val;

	size = size_of_envp(line + place + 1);
	env = malloc(sizeof(char) * (size + 1));
	if (!env)
		return (NULL);
	fill_env(line + place + 1, env, size);
	if (ary_strcmp(env, "?"))
	{
		env_val = ft_itoa(prev_ret);
		if (!env_val)
			return (NULL);
		return (rewrite_env(line, place, env_val, size + 1));
	}
	env_val = getenv(env);
	free(env);
	if (!env_val)
		return (remove_env(line, place, size + 1, par));
	else
		return (rewrite_env(line, place, env_val, size + 1));
}

char *envp_proc(char *line, int prev_ret, t_par *par)
{
	int ct;

	ct = 0;
	while (line[ct])
	{
		if (line[ct] == '\'')
		{
			ct += alt_quote_skip(line + ct);
			continue;
		}
		if (line[ct] == '$' && chek_back(line + ct, line) && chek_line(line[ct + 1]))
			return (replace_envp(line, ct, prev_ret, par));
		++ct;
	}
}

int is_com_sym(char *line)
{
	if (ary_strcmp("|", line))
		return (1);
	else if (ary_strcmp("<<", line))
		return (2);
	else if (ary_strcmp("<", line))
		return (3);
	else if (ary_strcmp(">>", line))
		return (4);
	else if (ary_strcmp(">", line))
		return (5);
	else
		return (false);
}

bool check_for_errors(t_com *com)
{
	t_snode *node;

	node = com->arg_start;
	if (ary_strcmp("|", com->arg_start->value))
		return (error_proc(error_syntax, "|"));
	while (node)
	{
		if (is_com_sym(node->value) && !node->next)
		{
			if (ary_strcmp("|", node->value))
				return (error_proc(error_syntax, "|"));
			else
				return (error_proc(error_syntax, "newline"));
		}
		if (is_com_sym(node->value) && is_com_sym(node->next->value))
			return (error_proc(error_syntax, node->value));
		node = node->next;
	}
	return (true);
}

int count_froks(t_com *com)
{
	int ct;
	t_snode *node;

	ct = 0;
	node = com->arg_start;
	if (node)
		++ct;
	while (node)
	{
		if (ary_strcmp("|", node->value))
			++ct;
		node = node->next;
	}
	return (ct);
}

void init_node(t_par *newnode)
{
	newnode->head = NULL;
	newnode->in_node = NULL;
	newnode->out_node = NULL;
	newnode->head = NULL;
	newnode->next = NULL;
	newnode->path = NULL;
	newnode->exe = NULL;
	newnode->heredoc = false;
	newnode->input_err = false;
	newnode->append = false;
	newnode->built_in = false;
	newnode->error = 0;
	newnode->fd_in = 0;
	newnode->fd_out = 1;
}

bool add_par_node(t_com *com)
{
	t_par *newnode;

	newnode = malloc(sizeof(t_par));
	if (!newnode)
		return (false);
	init_node(newnode);
	if (!com->par_head)
	{
		com->par_head = newnode;
		com->par_tail = newnode;
	}
	else
	{
		com->par_tail->next = newnode;
		com->par_tail = newnode;
	}
	return (true);
}

void free_par_slist(t_par *par)
{
	t_snode *node;

	while (par->head)
	{
		node = par->head;
		par->head = par->head->next;
		if (node->value)
			free(node->value);
		free(node);
	}
	par->head = NULL;
}

bool add_to_par_slist(t_par *par, void *value)
{
	t_snode *node;

	if (!value)
		return (false);
	node = malloc(sizeof(t_snode));
	if (!node)
	{
		free(value);
		return (false);
	}
	node->value = value;
	node->next = NULL;
	if (!par->head)
	{
		par->head = node;
		par->tail = node;
		return (true);
	}
	par->tail->next = node;
	par->tail = par->tail->next;
	return (true);
}

bool here_doc_input(char *limiter, t_par *par)
{
	char buffer[513];
	int ret;

	if (par->head)
		free_par_slist(par);
	// if (!limiter)
	// 	limiter = ary_strdup("");
	// if (!limiter)
	// 	return (false);
	write(1, "heredoc> ", 9);
	while (true)
	{
		ret = read(STDIN_FILENO, buffer, 512);
		if (ret < 0)
			return (false);
		if (!ret)
			continue;
		buffer[ret - 1] = '\0';
		if (ary_strcmp(buffer, limiter))
			return (true);
		buffer[ret - 1] = '\n';
		buffer[ret] = 0;
		if (!(add_to_par_slist(par, ary_strdup(buffer))))
			return (false);
		write(1, "heredoc> ", 9);
	}
}

bool here_doc(t_com *com)
{
	t_par *par;
	t_snode *node;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			par = par->next;
			node = node->next;
		}
		if (ary_strcmp("<<", node->value))
		{
			if (!here_doc_input(node->next->value, par))
				return (false);
			par->heredoc = true;
			node = node->next;
		}
		node = node->next;
	}
	return (true);
}

t_snode *skip_par(t_snode *node)
{
	while (node)
	{
		if (ary_strcmp("|", node->value))
			break;
		node = node->next;
	}
	return (node);
}

void check_in(t_com *com)
{
	t_snode *node;
	t_par *par;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			node = node->next;
			par = par->next;
		}
		if (ary_strcmp("<", node->value))
		{
			node = node->next;
			if (access(node->value, F_OK))
				par->error = error_nofile;
			else if (access(node->value, R_OK))
				par->error = error_noprem;
			if (par->error)
			{
				par->input_err = true;
				par->error_node = node;
			}
			node = skip_par(node);
			continue;
		}
		node = node->next;
	}
}

int make_file(char *name)
{
	int fd;

	fd = open(name, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

void setup_out(t_com *com)
{
	t_snode *node;
	t_par *par;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			node = node->next;
			par = par->next;
		}
		if (par->input_err || par->error == error_ambig)
		{
			node = skip_par(node);
			continue;
		}
		if ((ary_strcmp(">>", node->value) || ary_strcmp(">", node->value)))
		{
			node = node->next;
			if (access(node->value, F_OK))
			{
				if (!make_file(node->value) && !par->error)
				{
					par->error = error_create;
					par->error_node = node;
				}
			}
			else if (access(node->value, W_OK))
				if (!par->error)
				{
					par->error = error_noprem;
					par->error_node = node;
				}
			continue;
		}
		node = node->next;
	}
}

void find_last_in(t_snode *node, t_par *par)
{
	while (node)
	{
		if (ary_strcmp("|", node->value))
			return;
		if (ary_strcmp("<", node->value))
		{
			node = node->next;
			par->in_node = node;
			par->heredoc = false;
			continue;
		}
		if (ary_strcmp("<<", node->value))
		{
			node = node->next;
			par->heredoc = true;
			par->in_node = NULL;
			continue;
		}
		node = node->next;
	}
}

void find_last_out(t_snode *node, t_par *par)
{
	while (node)
	{
		if (ary_strcmp("|", node->value))
			return;
		if (ary_strcmp(">", node->value))
		{
			node = node->next;
			par->append = false;
			par->out_node = node;
			continue;
		}
		if (ary_strcmp(">>", node->value))
		{
			node = node->next;
			par->append = true;
			par->out_node = node;
			continue;
		}
		node = node->next;
	}
}

void setup_par(t_com *com)
{
	t_par *par;
	t_snode *node;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			node = node->next;
			par = par->next;
		}
		if (par->error)
		{
			node = skip_par(node);
			continue;
		}
		find_last_in(node, par);
		find_last_out(node, par);
		node = skip_par(node);
	}
}

bool prev_chek(t_snode *node)
{
	if (!node || ary_strcmp("|", node->value))
		return (true);
	if (ary_strcmp("<<", node->value) || ary_strcmp("<", node->value) || ary_strcmp(">>", node->value) || ary_strcmp(">", node->value))
		return (false);
	return (true);
}

void setup_exe(t_com *com)
{
	t_par *par;
	t_snode *node;

	node = com->arg_start;
	par = com->par_head;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			node = node->next;
			par = par->next;
		}
		if (par->error)
		{
			node = skip_par(node);
			continue;
		}
		if (prev_chek(node->prev) && node->value)
		{
			par->exe = node;
			node = skip_par(node);
			continue;
		}
		node = node->next;
	}
}

/* parses env until finds PATH  */

char *envp_parse(char **envp)
{
	int ct;

	ct = -1;
	while (envp[++ct])
		if (ary_strlen(envp[ct]) > 5 && envp[ct][0] == 'P' && envp[ct][1] == 'A' && envp[ct][2] == 'T' && envp[ct][3] == 'H' && envp[ct][4] == '=')
			break;
	return (envp[ct]);
}

char *find_path(t_par *par, char **paths, char *name)
{
	char *path;
	int ct;

	ct = -1;
	while (paths[++ct])
	{
		path = ft_strjoin(paths[ct], name);
		if (!path)
			return (NULL);
		if (!(access(path, F_OK)))
		{
			if (!(access(path, X_OK)))
			{
				par->error = 0;
				return (path);
			}
			par->error = error_noprem;
		}
		free(path);
	}
	par->error_node = par->exe;
	par->error = error_nocom;
	return (NULL);
}

bool setup_path(t_com *com)
{
	t_par *par;
	char *env_path;
	char **paths;

	par = com->par_head;
	while (par)
	{
		if (!par->exe)
		{
			par = par->next;
			continue;
		}
		if (!par->error && !access(par->exe->value, F_OK))
		{
			if (access(par->exe->value, X_OK))
			{
				par->error = error_noprem;
				continue;
			}
			else
			{
				par->path = ary_strdup(par->exe->value);
				if (!par->path)
					return (false);
			}
		}
		else /* we kinda need to re-do it with linked list */
		{
			env_path = envp_parse(com->envp);
			if (!env_path)
				par->error = error_noprem;
			else
			{
				paths = ft_split(env_path + 5, ':');
				if (!paths)
					return (false);
				par->path = find_path(par, paths, par->exe->value);
				if (!par->path && par->error != error_nocom)
					return (false);
			}
		}
		par = par->next;
	}
	return (true);
}

bool add_to_argv_list(t_argv_list *list, void *val)
{
	t_snode *newnode;

	newnode = malloc(sizeof(t_snode));
	if (!newnode)
		return (NULL);
	newnode->value = val;
	newnode->next = NULL;
	if (!list->head)
	{
		list->head = newnode;
		list->tail = newnode;
		++list->size;
		return (true);
	}
	list->tail->next = newnode;
	list->tail = newnode;
	++list->size;
	return (true);
}

char **list_to_char(t_argv_list *list, char *head)
{
	char **ret;
	int ct;
	t_snode *temp;

	ret = malloc(sizeof(char *) * (++list->size + 1));
	if (!ret)
		return (NULL);
	ret[list->size] = NULL;
	ret[0] = head;
	ct = 0;
	while (++ct < list->size)
	{
		temp = list->head;
		ret[ct] = temp->value;
		list->head = list->head->next;
		free(temp);
	}
	list->size = 0;
	list->head = NULL;
	return (ret);
}

bool setup_argv(t_com *com)
{
	t_snode *node;
	t_par *par;
	t_argv_list list;

	list.head = NULL;
	par = com->par_head;
	node = com->arg_start;
	list.size = 0;
	while (node)
	{
		if (ary_strcmp("|", node->value))
		{
			node = node->next;
			if (par->exe)
			{
				par->argv = list_to_char(&list, par->exe->value);
				if (!par->argv)
					return (false);
			}
			par = par->next;
		}
		if (par->error)
		{
			node = skip_par(node);
			continue;
		}
		if (prev_chek(node->prev))
			if (node != par->exe && !is_com_sym(node->value))
				if (!add_to_argv_list(&list, node->value))
					return (false);
		node = node->next;
	}
	if (par->exe)
	{
		par->argv = list_to_char(&list, par->exe->value);
		if (!par->argv)
			return (false);
	}
	return (true);
}

// bool read_to_fd(char *name, int fd)
// {
// 	char buffer[513];
// 	int		name_fd;
// 	int		ret;

// 	name_fd = open(name, O_RDONLY);
// 	if (name_fd < 0)
// 		return(false);
// 	while (true)
// 	{
// 		ret = read(name_fd, buffer, 512);
// 		if (ret == 0)
// 			break ;
// 		if (ret < 0)
// 			return(false);
// 		buffer[ret] = '\0';
// 		write(fd, buffer , ret);
// 	}
// 	close(name_fd);
// 	return(true);
// }

void here_to_fd(t_snode *head, int fd)
{
	while (head)
	{
		write(fd, head->value, ary_strlen(head->value));
		head = head->next;
	}
}

// bool fill_in_pipes(t_com *com)
// {
// 	t_par	*par;
// 	int		t_pipe[2];

// 	par = com->par_head;
// 	while (par)
// 	{
// 		if (par->error || !par->in_node)
// 		{
// 			par = par->next;
// 			continue ;
// 		}
// 		if (!wr_pipe(com, t_pipe))
// 		{
// 			if (t_pipe[0] == -1 && t_pipe[1] == -1)
// 			{
// 				par->error = error_open;
// 				par = par->next;
// 				continue ;
// 			}
// 			else
// 				return(false);
// 		}
// 		if (!par->heredoc && !read_to_fd(par->in_node->value, t_pipe[1]))
// 			{
// 				par->error = error_open;
// 				par = par->next;
// 				continue ;
// 			}
// 		else if (par->heredoc)
// 			here_to_fd(par->head, t_pipe[1]);
// 		par->fd_in = t_pipe[0];
// 		par = par->next;
// 	}

// }

bool reform_nodes(t_com *com)
{
	int size;
	int ct;

	size = 0;
	if (!check_for_errors(com))
		return (false);
	// size = count_froks(com);
	// com->par_head = NULL;
	// ct = -1;
	// while (++ct < size)
	// 	if (!add_par_node(com))
	// 		return (error_proc(error_malloc, NULL));
	if (!here_doc(com))
		return (error_proc(error_malloc, NULL));
	check_in(com);
	setup_out(com);
	setup_par(com);
	setup_exe(com);
	if (!setup_path(com))
		return (error_proc(error_malloc, NULL));
	if (!setup_argv(com))
		return (error_proc(error_malloc, NULL));
	// if (!fill_in_pipes(com))
	// 	return (error_proc(error_malloc, NULL));
	return (true);
}

bool i_o_pipework(t_com *com, t_par *par)
{
	if (par->next)
	{
		if (!wr_pipe(com, com->p))
		{
			if (com->p[0] == -1 && com->p[1] == 1)
				return (err_set(par, error_pipe));
			else
				return (err_set(par, error_malloc));
		}
		if (!par->out_node)
			par->fd_out = com->p[1];
		else
			wr_close(com, com->p[1]);
		par->next->fd_in = com->p[0];
	}
	if (par->heredoc)
	{
		if (!wr_pipe(com, com->p))
		{
			if (com->p[0] == -1 && com->p[1] == 1)
				return (err_set(par, error_pipe));
			else
				return (err_set(par, error_malloc));
		}
		here_to_fd(par->head, com->p[1]);
		wr_close(com, com->p[1]);
		par->fd_in = com->p[0];
	}
	return (true);
}

bool i_o_setup(t_com *com, t_par *par)
{
	if (!i_o_pipework(com, par))
		return (false);
	else if (par->in_node)
	{
		par->fd_in = wr_open(com, par->in_node->value, O_RDONLY);
		if (par->fd_in == -1)
			return (err_set(par, error_open));
		if (par->fd_in == -2)
			return (err_set(par, error_malloc));
	}
	if (par->out_node)
	{
		if (par->append)
			par->fd_out = wr_open(com, par->out_node->value, O_WRONLY | O_APPEND);
		else
			par->fd_out = wr_open(com, par->out_node->value, O_WRONLY | O_TRUNC);
		if (par->fd_out == -1)
			return (err_set(par, error_open));
		if (par->fd_out == -2)
			return (err_set(par, error_malloc));
	}
	return (true);
}

bool add_pid(t_com *com, int pid)
{
	t_pid_node *newnode;

	newnode = malloc(sizeof(t_pid_node));
	if (!newnode)
		return (false);
	newnode->pid = pid;
	newnode->next = NULL;
	if (!com->pw_list.head)
	{
		com->pw_list.head = newnode;
		com->pw_list.tail = newnode;
		return (true);
	}
	com->pw_list.tail->next = newnode;
	com->pw_list.tail = newnode;
	return (true);
}

bool execute_par(t_com *com, t_par *par)
{
	if (!i_o_setup(com, par))
		return (false);
	par->pid = fork();
	if (par->pid < 0)
		return (err_set(par, error_fork));
	if (!par->pid) /*child */
	{
		if (par->fd_in != 0 && dup2(par->fd_in, STDIN_FILENO) == -1)
			return (err_set(par, error_dupe)); /* dunno about child err mngmnt */ /* TODO : PROPPER ERR MNGMNT */
		if (par->fd_out != 1 && dup2(par->fd_out, STDOUT_FILENO) == -1)
			return (err_set(par, error_dupe));
		close_inhereted(com, par->fd_in, par->fd_out);
		if (execve(par->path, par->argv, com->envp) == -1)
			return (err_set(par, error_execve));
	}
	else /*main */
	{
		if (!add_pid(com, par->pid))
			return (err_set(par, error_malloc));
		if (par->fd_in != 0)
			wr_close(com, par->fd_in);
		if (par->fd_out != 1)
			wr_close(com, par->fd_out);
	}
}

bool wait_all_pids(t_com *com)
{
	t_pid_node *node;

	while (com->pw_list.head)
	{
		node = com->pw_list.head;
		if (waitpid(node->pid, NULL, 0) == -1) /*maybe add som flags and shieet */
			return (false);
		if (WIFEXITED(node->pid))
			com->prev_ret = WIFEXITED(node->pid);
		else
			com->prev_ret = 0;
		com->pw_list.head = com->pw_list.head->next;
		free(node);
	}
	return (true);
}

bool execute_pipeline(t_com *com)
{
	t_par *par;

	par = com->par_head;
	while (par)
	{
		if (par->error) /*  test output */
		{
			printf("Error num - %d\n", par->error);
			if (par->error_node)
				printf("Error in \"%s\"\n", (char *)par->error_node->value);
			par = par->next;
			continue;
		}
		if (!par->exe)
		{
			if (par->next && !par->next->in_node)
			{
				if (!wr_pipe(com, com->p))
				{
					if (com->p[0] == -1 && com->p[1] == 1)
						return (err_set(par, error_pipe));
					else
						return (err_set(par, error_malloc));
				}
				wr_close(com, com->p[1]);
				par->next->fd_in = com->p[0];
			}
			par = par->next;
			continue;
		}
		// if (par->built_in)
		// 	;//your func  exec here
		else if (!execute_par(com, par))
			continue;
		par = par->next;
	}
	if (!wait_all_pids(com))
		return (error_proc(error_waitpid, NULL));
	close_inhereted(com, 0, 1);
	return (true);
}

void free_all(t_com *com)
{
	t_par *par;

	while (com->par_head)
	{
		par = com->par_head;
		com->par_head = par->next;
		if (par->head)
			free_snode(par->head);
		free(par->path);
		free(par->argv);
		free(par->line);
		if (par->error == error_ambig)
		{
			free(par->error_node->value);
			free(par->error_node);
		}
		free(par);
	}
	free_snode(com->arg_start);
	com->par_head = NULL;
	com->arg_start = NULL;
}

char *sq_qutes(char *line)
{
	char spec;
	char *start;

	spec = *line;
	++line;
	start = line;
	if (!*line)
		return (line);
	while (*line)
	{
		if (*line = spec)
			return (++line);
		++line;
	}
	return (start);
}

bool line_to_par(t_com *com, char *line)
{
	if (*line)
		if (!add_par_node(com))
			return (false);
	while (*line)
	{
		line = trim_space(line);
		if (*line == '\'' || *line == '\"')
		{
			line = sq_qutes(line);
			continue;
		}
		if (*line == '|')
			if (!add_par_node(com))
				return (false);
		++line;
	}
	return (true);
}

int qute_size(char *line)
{
	char spec;
	int size;

	spec = *line;
	++line;
	size = 1;
	while (*line)
	{
		if (*line == spec)
			return (++size);
		++size;
		++line;
	}
	return (1);
}

int line_size(char *line)
{
	int size;
	int quote;

	size = 0;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			quote = qute_size(line);
			size += quote;
			line += quote;
			continue;
		}
		if (*line == '|')
			return (size);
		++size;
		++line;
	}
	return (size);
}

void fill_line(char *newline, char *line, int size)
{
	int ct;

	ct = -1;
	while (++ct < size)
		newline[ct] = line[ct];
	newline[ct] == '\0';
}

bool split_line_to_pars(t_com *com, char *line)
{
	t_par *par;
	int size;
	char *newline;

	par = com->par_head;
	while (par)
	{
		size = line_size(line);
		newline = malloc(sizeof(char) * (size + 1));
		if (!newline)
			return (false);
		fill_line(newline, line, size);
		par->line = newline;
		line += size + 1; // +1????
		par = par->next;
	}
	return (true);
}

bool chek_empty(char *line)
{
	line = trim_space(line);
	if (!*line)
		return (true);
	else
		return (false);
}

bool par_envp(t_com *com)
{
	t_par *par;
	int ct;
	int envc;

	par = com->par_head;
	while (par)
	{
		envc = count_envp(par->line);
		ct = -1;
		while (++ct < envc)
		{
			par->line = envp_proc(par->line, com->prev_ret, par);
			if (!par->line)
				return (false);
			if (chek_empty(par->line))
			{
				free(par->line);
				par->line = NULL;
			}
		}
		par = par->next;
	}
	return (true);
}

bool par_to_parse(t_com *com)
{
	t_par *par;
	char *duct;

	par = com->par_head;
	while (par)
	{
		if (!par->line)
		{
			if (!add_snode(com, NULL))
				return (error_proc(error_malloc, NULL));
		}
		else if (!parse_line(com, par->line))
			return (error_proc(error_malloc, NULL));
		if (par->next)
		{
			duct = ary_strdup("|");
			if (!duct)
				return (error_proc(error_malloc, NULL));
			if (!add_snode(com, duct))
				return (error_proc(error_malloc, NULL));
		}
		par = par->next;
	}
	return (true);
}

int main(int argc, char **argv, char **envp)
{
	t_com com;
	char *line;
	char *test_line = "ls | $asd | cat";

	com.arg_start = NULL;
	com.pw_list.head = NULL;
	com.inh_list.head = NULL;
	com.par_head = NULL;
	com.envp = envp;
	line = ary_strdup(test_line);
	if (!line || !*line)
		return (-1);
	com.prev_ret = 0;
	if (!line_to_par(&com, line))
		return (error_proc(error_malloc, NULL));
	if (!split_line_to_pars(&com, line))
		return (error_proc(error_malloc, NULL));
	if (!par_envp(&com))
		return (error_proc(error_malloc, NULL));
	free(line);
	par_to_parse(&com);
	print_node(&com);
	reform_nodes(&com);
	execute_pipeline(&com);
	free_all(&com);
	return (0);
}
