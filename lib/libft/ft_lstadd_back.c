/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:01:27 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:19:30 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list *last;

    if (!lst || !new)
        return;

    if (*lst == NULL)
        (*lst) = new;
    else 
    {
        last = ft_lstlast((*lst));
        last->next = new;
    }
}