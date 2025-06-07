/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:02:06 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:18:45 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void	*ft_calloc(size_t nelem, size_t size)
{
	unsigned char	*tab;

	tab = (unsigned char *)malloc(size * nelem);
	if (!tab)
		return (NULL);
	ft_bzero(tab, (nelem * size));
	return ((void *)tab);
}
