/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:01:27 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:19:35 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void ft_lstadd_front(t_list **lst, t_list *new)
{
    t_list *current;

    current = (*lst);
    (*lst) = new;
    (*lst)->next = current;
}