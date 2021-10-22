#include "header.h"

bool error_proc(int error)
{
	exit(-1);
	return (false);
}

char *trim_space(char *line)
{
	while (*line == ' ')
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

	size = 0;
	while (*line)
	{
		if (*line == ' ')
			break;
		if (*line == '\'' || *line == '\"')
		{
			line = quote(line, &size);
			continue ;
		}
		if (*line == '\\')
			++line; //wrong segfault if '\' is last  // seems fixed
		if (!*line)
			break;
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
	bool safe;

	ct = -1;
	newline = malloc(sizeof(char) * (size + 1));
	if (!newline)
		return (NULL);
	while (++ct < size)
	{
		safe = false;
		if (*line == '\\')
		{
			safe = true;
			++line;
		}
		if (!safe && (*line == '\'' || *line == '\"'))
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

	line = trim_space(line);
	if (!*line)
		return (false); // todo: some logic for emptiline;
	while (*line)
	{
		size = get_size(line);
		if (!size)
			return(true);
		line = add_arg(com, line, size);
		if (!line)
			return (error_proc(error_malloc));
		line = trim_space(line);
	}
	return(true);
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
		printf("%s\n", (char *)temp->value);
		temp = temp->next;
	}
}

int ary_strlen(char *s)
{
	int ct;

	ct = -1;
	while (s[++ct])
		;
	return (ct);
}

char *ary_strdup(char *src)
{
	char *ret;
	int len;

	len = ary_strlen(src);
	ret = malloc(sizeof(char) * ++len);
	if (!ret)
		return (NULL);
	while (len-- > 0)
		ret[len] = src[len];
	return (ret);
}

char *quote_skip(char *line)
{
	char	spec;
	char	*base;

	spec = *line;
	++line;
	base = line;
	while (*line)
	{
		if (*line == spec)
			return(++line);
		++line;
	}
	return(base);
}

char *skip_env(char *line)
{
	++line;
	while (*line)
	{
		if (*line == '\\' || *line == '\"' || *line == '\'' || *line == ' ' || *line == '$')
			return(line);
		++line;
	}
	return(line);
}

int count_envp(char *line)
{
	int ret;
	bool safe;

	ret = 0;
	while (*line)
	{
		safe = false;
		if (*line == '\\')
		{
			safe = true;
			++line;
		}
		if(!line)
			return(ret);
		if (!safe && *line == '\'')
		{
			line = quote_skip(line);
			continue ;
		}
		if (!safe && *line == '$' && line[1] != '\0' && line[1] != '\\' && line[1] != ' ')
		{
			++ret;
			line = skip_env(line);
			continue ;
		}
		++line;
	}
	return(ret);
}

int alt_quote_skip(char *line)
{
	int ct;
	char spec;

	ct = -1;
	spec = *line;
	++line;
	if (*line == spec)
		return(1);
	while (line[++ct])
		if (line[ct] == spec)
			return(ct + 2);
	return(1);
}

int size_of_envp(char *line)
{
	int ct;

	ct = -1;
	while (line[++ct])
		if (line[ct] == '\\' || line[ct] == '\'' || line[ct] == '\"' || line[ct] == ' ')
			return(ct);
	return(ct);
}

void fill_env(char *line, char *env, int size)
{
	int ct;

	ct = -1;
	while (++ct < size)
		env[ct] = line[ct];
	env[ct] = '\0';
}

char *remove_env(char *line, int place, int size)
{
	int len;
	char *newline;
	int ct;

	len = ary_strlen(line) - size;
	newline = malloc(sizeof(char) * (len + 1));
	if(!newline)
		return(NULL);
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
	return(newline);		
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
	newline = malloc(sizeof(char) * (len + val_len - size +1));
	if(!newline)
		return(NULL);
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
	free(env_val);
	return(newline);
}

char *replace_envp(char *line, int place)
{
	int		size;
	char	*env;
	char	*env_val;

	size = size_of_envp(line + place + 1);
	env = malloc(sizeof(char) * (size + 1));
	if (!env)
		return(NULL);
	fill_env(line + place + 1 , env, size);
	env_val = getenv(env);
	if (!env_val)
		return(remove_env(line, place, size + 1));
	else
		return(rewrite_env(line, place, env_val, size + 1));
}

char *envp_proc(char *line)
{
	int		ct;
	bool	safe;

	ct = 0;
	while (line[ct])
	{
		safe = false;
		if (line[ct] == '\\')
		{
			safe = true;
			++ct;
		}
		if (!safe && line[ct] == '\'')
		{
			ct += alt_quote_skip(line + ct);
			continue ;
		}
		if(!safe && line[ct] == '$' && line[ct + 1] != ' ' && line[ct + 1] != '\\' && line[ct + 1] != '\0')
			return(replace_envp(line, ct));
		++ct;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_com com;
	char *line;
	size_t len;
	char **args;
	int ct;
	int envc;
	(void)argc;
	(void)argv;

	// char *test_line = "asd\"\\$USER ary\"test";
	char *test_line = "\\$USER asd\"\\$USER  $ $USER\"test\'$USER asd hehe' $a $\\ asd \"\" \'\'";
	// len = 0;
	// while (true)
	line = ary_strdup(test_line);
	if (!line)
		return (-1);
	// {
	// ret = getline(&line, &len, stdin);
	envc = count_envp(line);
	ct = -1;
	while (++ct < envc)
	{
		line = envp_proc(line);
		if (!line)
			return (error_proc(error_malloc));
		// printf("%s\n", line);
	}
	com.arg_start = NULL;
	printf("%s\n", line);
	// if (ret == -1)
	// 	return (0);
	parse_line(&com, line);
	// write(1, line, len);
	free(line);
	print_node(&com);
	free_snode(com.arg_start);
	line = NULL;
	// }
	return (0);
}
