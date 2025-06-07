/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:03:02 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:21:13 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;
	int	j;

	i = 0;
	if (!f || !s)
		return ;
	j = (int)ft_strlen(s);
	if (s != NULL && f != NULL)
	{
		while (i < j && s != NULL && f != NULL)
		{
			(*f)(i, s);
			i++;
			s++;
		}
	}
}
