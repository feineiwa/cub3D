/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 08:53:26 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/07 13:21:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

void	push_in_stash(t_gnl **stash, char *buff)
{
	t_gnl	*new_list;
	t_gnl	*last_list;

	new_list = malloc(sizeof(t_gnl));
	last_list = ft_lstlast_gnl(*stash);
	if (new_list == NULL)
		return ;
	if (*stash == NULL)
		*stash = new_list;
	else
		last_list->next = new_list;
	new_list->content = buff;
	new_list->next = NULL;
}

t_gnl	*ft_lstlast_gnl(t_gnl *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

t_gnl	*ft_lstnew_gnl(void *content)
{
	t_gnl	*new;

	new = (t_gnl *)malloc(sizeof(t_gnl));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstclear_gnl(t_gnl **lst)
{
	t_gnl	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
}

int	found_new_line(t_gnl *stash)
{
	int	i;

	if (!stash)
		return (0);
	while (stash)
	{
		i = 0;
		while (stash->content[i] != '\0')
		{
			if (stash->content[i] == '\n')
				return (1);
			i++;
		}
		stash = stash->next;
	}
	return (0);
}
