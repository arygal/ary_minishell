/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maearly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 15:00:50 by maearly           #+#    #+#             */
/*   Updated: 2021/11/09 15:00:51 by maearly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_malloc(size_t size)
{
	void	*alloc;

	alloc = malloc(size);
	if (!alloc)
	{
		write(2, "Malloc error!\n", ft_strlen("Malloc error!\n"));
		exit(1);
	}
	return (alloc);
}
