/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:03:19 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:21:40 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*s2 == '\0')
		return ((char *)s1);
	while (s1[i] && i < len)
	{
		j = 0;
		while (s1[i + j] == s2[j] && s2[j] && (i + j) < len)
			j++;
		if (s2[j] == '\0')
			return ((char *)&s1[i]);
		i++;
	}
	return (NULL);
}
