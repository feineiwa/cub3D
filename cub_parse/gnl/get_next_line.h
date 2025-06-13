/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:49:10 by eralambo          #+#    #+#             */
/*   Updated: 2025/05/12 13:58:18 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char	*ft_strjoin_mod(char *s1, char *s2);
char	*ft_strdup_gnl(char *str);
char	*ft_rest_str(char *str);
char	*ft_fgets(char *str);
char	*ft_read(int fd, char *str);
char	*get_next_line(int fd);
char	*ft_if_else(char *str, char *buff);

size_t	ft_find_newline(char *str);
size_t	ft_strlen(const char *s);

#endif
