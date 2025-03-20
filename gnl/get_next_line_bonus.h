/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 08:56:51 by frahenin          #+#    #+#             */
/*   Updated: 2025/03/07 13:23:17 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_gnl
{
	char			*content;
	struct s_gnl	*next ;
}	t_gnl;

char	*get_next_line(int fd);
void	push_in_stash(t_gnl **stash, char *buff);
t_gnl	*ft_lstlast_gnl(t_gnl *lst);
t_gnl	*ft_lstnew_gnl(void *content);
void	ft_lstclear_gnl(t_gnl **lst);
int		found_new_line(t_gnl *stash);

#endif
