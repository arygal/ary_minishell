/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megen <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:39:33 by megen             #+#    #+#             */
/*   Updated: 2021/11/06 14:39:37 by megen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/* makes new str and copies content of two strings in it */

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	a;
	size_t	b;
	size_t	ct;
	char	*ret;

	a = ary_strlen(s1);
	b = ary_strlen(s2);
	ret = malloc(a + b + 1);
	if (!ret)
		return ((void *)0);
	ct = -1;
	while (++ct < a)
		ret[ct] = s1[ct];
	ct = -1;
	while (++ct < b)
		ret[a + ct] = s2[ct];
	ret[a + ct] = '\0';
	return (ret);
}

/* strlen */

int ary_strlen(char *s)
{
	int ct;

	ct = -1;
	while (s[++ct])
		;
	return (ct);
}

/* strdup */

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

/* strcmp but in boolean */

bool	ary_strcmp(char *src, char *trgt)
{
	if ((!src && !trgt) || (src == trgt))
		return (true);
	if (!src || !trgt)
		return (false);
	while (*src && *trgt)
	{
		if (*src != *trgt)
			return (false);
		++src;
		++trgt;
	}	
	if (*src != *trgt)
		return (false);
	return (true);
}