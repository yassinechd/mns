/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yass <yass@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:01:27 by yass              #+#    #+#             */
/*   Updated: 2025/04/23 18:19:39 by yass             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void ft_lstclear(t_list **lst, void (*del)(void *))
{
    t_list *current;
    
    if (!lst || !del)
        return;
    while (*lst)
    {
        current = *lst;
        *lst = (*lst)->next;
        ft_lstdelone(current, del);
    }
    *lst = NULL;
}
