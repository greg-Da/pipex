/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_custom_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: greg <greg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:08:24 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/05 13:58:13 by greg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_set_to_zero(int *a, int *b, int *c, int *d)
{
	*a = 0;
	*b = 0;
	*c = 0;
	*d = 0;
}

int	ft_count_words(char const *s, char c)
{
	int			count;
	int			in_word;
	t_quotes	quotes;
	const char	*start;

	ft_set_to_zero(&count, &in_word, &quotes.d_quotes, &quotes.s_quotes);
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
			in_word = 0;
		s++;
	}
	return (count);
}

void	ft_check_quotes(char **arr, t_custom_split *stru, t_pipex *pipex)
{
	if (arr[stru->j][0] == '\'' || arr[stru->j][0] == '\"')
	{
		arr[stru->j][ft_strlen(arr[stru->j]) - 1] = 0;
		ft_strlcpy(arr[stru->j], &arr[stru->j][1], ft_strlen(arr[stru->j]));
	}
	if (arr[stru->j][0] == '\"')
	{
		free(arr[stru->j]);
		arr[stru->j] = ft_strdup("{print}");
	}
	if (arr[stru->j][0] == '\'')
		pipex->exit_code = 1;
}

size_t	ft_get_str(const char *s, char c, t_custom_split *stru, char **arr)
{
	size_t	next;

	while (s[stru->i] == c)
		stru->i++;
	next = ft_next_occurence(s, c, stru->i) - stru->i;
	arr[stru->j] = ft_substr(s, (unsigned int)stru->i, next);
	return (next);
}

char	**ft_custom_split(char const *s, char c, t_pipex *pipex)
{
	t_custom_split	stru;
	char			**arr;
	size_t			next;

	if (!s)
		return (NULL);
	stru.i = 0;
	stru.j = -1;
	stru.count = ft_count_words(s, c);
	arr = malloc((stru.count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (++stru.j < stru.count)
	{
		next = ft_get_str(s, c, &stru, arr);
		ft_check_quotes(arr, &stru, pipex);
		if (!arr[stru.j])
			return (ft_free(arr, stru.j));
		arr[stru.j] = ft_remove_slash(arr[stru.j], ft_strlen(arr[stru.j]));
		stru.i += next;
	}
	arr[stru.j] = NULL;
	return (arr);
}
