/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <maearly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 16:03:43 by maearly           #+#    #+#             */
/*   Updated: 2020/10/30 19:29:41 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	w_count(char const *s, char c)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			++i;
		if (s[i] && s[i] != c)
			++len;
		while (s[i] && s[i] != c)
			++i;
	}
	return (len);
}

static char	**free_mem(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		w_len;
	int		i;
	int		j;

	strs = (char **)malloc(sizeof(char *) * (w_count(s, c) + 1));
	if (!strs || !s)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < w_count(s, c))
	{
		while (s[i] && s[i] == c)
			++i;
		w_len = 0;
		while (s[i + w_len] && s[i + w_len] != c)
			++w_len;
		strs[j] = (char *)malloc(sizeof(char) * (w_len + 1));
		if (!strs[j])
			return (free_mem(strs));
		ft_strlcpy(strs[j++], s + i, w_len + 1);
		i += w_len;
	}
	strs[j] = NULL;
	return (strs);
}
