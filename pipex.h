/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:21:58 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/04 19:19:29 by gdalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include "./libft/libft.h"

typedef struct s_pipex
{
	int		*pids;
	int		pids_size;
	int		in_fd;
	int		out_fd;
	char	**cmd_path;
	char	***cmd_args;
	int		here_doc;
	int		fd[2];
	int		exit_code;
	int		cmd_count;
	int		is_invalid_infile;
	// int		is_invalid_outfile;
}	t_pipex;

typedef struct s_prev
{
	int		in;
	int		out;
	int		i;
}	t_prev;

typedef struct s_split
{
	int	count;
	int	i;
	int	j;
}	t_custom_split;

typedef struct s_quotes
{
	int	s_quotes;
	int	d_quotes;
}	t_quotes;

void	ft_init_struct(t_pipex *pipex, int ac, char **av, char **envp);
int		ft_here_doc(int fd, char *limiter);
void	ft_error(char *str);
char	**ft_custom_split(char const *s, char c, t_pipex *pipex);

#endif