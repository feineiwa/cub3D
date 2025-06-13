/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eralambo <eralambo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:15:59 by eralambo          #+#    #+#             */
/*   Updated: 2024/03/16 22:51:35 by eralambo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_get_nbr_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			count++;
			while (*s != c && *s != '\0')
				s++;
		}
	}
	return (count);
}

static void	ft_free(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		s[i] = NULL;
		i++;
	}
	free(s);
	s = NULL;
}

static size_t	ft_spliting(const char *s, char **words, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;

	k = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			j = i;
			while (s[j] != c && s[j])
				j++;
			words[k++] = ft_substr(s, i, j - i);
			if (!words)
				ft_free(words);
			i = j;
		}
	}
	return (k);
}

char	**ft_split(const char *s, char c)
{
	char	**words;
	size_t	i;
	size_t	k;

	k = 0;
	i = ft_get_nbr_words(s, c);
	words = (char **)malloc(sizeof(char *) * (i + 1));
	if (!words)
		return (NULL);
	k = ft_spliting(s, words, c);
	if (!words)
	{
		ft_free(words);
		return (NULL);
	}
	words[k] = NULL;
	return (words);
}
