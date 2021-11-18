/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <maearly@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 13:24:28 by maearly           #+#    #+#             */
/*   Updated: 2020/10/30 19:29:43 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*buf;
	int		i;

	if (!s)
		return (NULL);
	buf = (char *)malloc(sizeof(char) * len + 1);
	if (NULL == buf)
		return (NULL);
	i = 0;
	while (start < ft_strlen(s) && len--)
	{
		buf[i] = s[i + start];
		i++;
	}
	buf[i] = '\0';
	return (buf);
}
