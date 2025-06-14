/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:44:47 by eralambo          #+#    #+#             */
/*   Updated: 2025/05/12 13:57:48 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_if_else(char *str, char *buff)
{
	if (!str)
		str = ft_strdup_gnl(buff);
	else
		str = ft_strjoin_mod(str, buff);
	return (str);
}

char	*ft_read(int fd, char *str)
{
	char	*buff;
	int		len_buff;
	size_t	i;

	i = 0;
	len_buff = 1;
	while (len_buff != 0 && i == 0)
	{
		buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buff)
			return (NULL);
		len_buff = read(fd, buff, BUFFER_SIZE);
		if (len_buff < 0)
		{
			free(buff);
			if (str)
				free(str);
			return (NULL);
		}
		buff[len_buff] = '\0';
		str = ft_if_else(str, buff);
		i = ft_find_newline(str);
	}
	return (str);
}

char	*ft_fgets(char *str)
{
	char	*line;
	size_t	i;
	size_t	j;

	if (!str || !(*str))
		return (NULL);
	j = 0;
	i = ft_find_newline(str);
	if (i == 0)
		i = ft_strlen(str);
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	while (j < i)
	{
		line[j] = str[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

char	*ft_rest_str(char *str)
{
	char	*rest;
	size_t	i;
	size_t	len_rest;
	size_t	j;

	j = 0;
	i = ft_find_newline(str);
	if (!str[i])
		return (free(str), NULL);
	len_rest = ft_strlen(str) - i;
	rest = malloc(sizeof(char) * (len_rest + 1));
	if (!rest)
		return (NULL);
	while (i < ft_strlen(str))
	{
		rest[j] = str[i];
		j++;
		i++;
	}
	rest[j] = '\0';
	free(str);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*res;

	res = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = ft_read(fd, str);
	if (!str)
		return (NULL);
	res = ft_fgets(str);
	if (ft_find_newline(str) > 0)
		str = ft_rest_str(str);
	else
	{
		free(str);
		str = NULL;
	}
	return (res);
}
