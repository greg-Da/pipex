/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_custom_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:08:24 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/03 17:08:27 by gdalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_count_words(char const *s, char c)
{
	int			count;
	int			in_word;
	t_quotes	quotes;
	const char	*start;

	count = 0;
	in_word = 0;
	quotes.s_quotes = 0;
	quotes.d_quotes = 0;
	start = s;
	while (*s)
	{
		if (*s == '\'' && (s == start || *(s - 1) != '\\'))
			quotes.s_quotes = !quotes.s_quotes;
		if (*s == '\"' && (s == start || *(s - 1) != '\\'))
			quotes.d_quotes = !quotes.d_quotes;
		if (!quotes.s_quotes && !quotes.d_quotes && *s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (!quotes.s_quotes && !quotes.d_quotes && *s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static size_t	ft_next_occurence(char const *s, char c, int index)
{
	t_quotes	quotes;
	
	quotes.s_quotes = 0;
	quotes.d_quotes = 0;
	while (s[index])
	{
		if (s[index] == '\'' && (index == 0 || s[index - 1] != '\\'))
			quotes.s_quotes = !quotes.s_quotes;
		if (s[index] == '\"' && (index == 0 || s[index - 1] != '\\'))
			quotes.d_quotes = !quotes.d_quotes;
		if (!quotes.s_quotes && !quotes.d_quotes && s[index] == c)
			break;
		index++;
	}
	return ((size_t)index);
}

static char	**ft_free(char **arr, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

char	**ft_custom_split(char const *s, char c, t_pipex *pipex)
{
	t_custom_split	t_split;
	char			**arr;

	if (!s)
		return (NULL);
	t_split.i = 0;
	t_split.j = -1;
	t_split.count = ft_count_words(s, c);
	arr = malloc((t_split.count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (++t_split.j < t_split.count)
	{
		while (s[t_split.i] == c)
			t_split.i++;
		arr[t_split.j] = ft_substr(s, (unsigned int)t_split.i, ft_next_occurence(s, c, t_split.i) - t_split.i);
		if (arr[t_split.j][0] == '\'' || arr[t_split.j][0] == '\"')
		{
			arr[t_split.j][ft_strlen(arr[t_split.j]) - 1] = 0;
			ft_strlcpy(arr[t_split.j], &arr[t_split.j][1], ft_strlen(arr[t_split.j]));
		}
		if (arr[t_split.j][0] == '\"')
		{
			free(arr[t_split.j]);
			arr[t_split.j] = ft_strdup("{print}");
		}
		if (arr[t_split.j][0] == '\'')
		{
			pipex->exit_code = 1;
		}
		
		if (!arr[t_split.j])
			return (ft_free(arr, t_split.j));
		t_split.i += ft_next_occurence(s, c, t_split.i) - t_split.i;

	}
	arr[t_split.j] = NULL;
	return (arr);
}