/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:17:12 by yachadla          #+#    #+#             */
/*   Updated: 2025/04/23 18:21:54 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last;

	i = 0;
	last = NULL;
	while (s[i])
		i++;
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	while (i >= 0)
	{
		if (s[i] == (char)c)
		{
			last = (char *)&s[i];
			break ;
		}
		i--;
	}
	return (last);
}
