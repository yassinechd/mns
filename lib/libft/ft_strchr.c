/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:02:58 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:21:02 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strchr(const char *s, int c)
{
	int					i;
	unsigned char		ca;
	char				*str;

	ca = (unsigned char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == ca)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == ca)
		return ((char *)&str[i]);
	return (NULL);
}
