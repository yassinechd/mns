/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:02:31 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:20:02 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*cara;
	unsigned char		char_c;

	i = 0;
	cara = (const unsigned char *)s;
	char_c = (unsigned char)c;
	while (i < n)
	{
		if (cara[i] == char_c)
			return ((void *)(cara + i));
		i++;
	}
	return (NULL);
}
