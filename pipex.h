#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>

# include "./libft/libft.h"

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	char	**cmd_path;
	char	***cmd_args;
	int		is_invalid_infile;
	int		here_doc;
	// int	cmd_count;
}	t_pipex;


#endif