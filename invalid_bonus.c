/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: greg <greg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:55:24 by greg              #+#    #+#             */
/*   Updated: 2024/12/05 14:59:25 by greg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_cmd_not_acc(char *name)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strjoin("pipex: line 1: ", name);
	tmp2 = ft_strjoin(tmp, ": command not found\n");
	ft_putstr_fd(tmp2, 2);
	free(tmp);
	free(tmp2);
}

void	ft_invalid_cmd(t_pipex *pipex, t_prev *prev)
{
	if (prev->i == pipex->cmd_count - 1)
		pipex->exit_code = 127;
	close(pipex->fd[1]);
	close(pipex->fd[0]);
}

int	ft_invalid_infile(t_pipex *pipex, t_prev *prev)
{
	if (pipex->is_invalid_infile == 1)
	{
		ft_putstr_fd("No such file or directory\n", 2);
		if (prev->i == pipex->cmd_count - 1)
			write(pipex->out_fd, "\0", 1);
		else
			write(pipex->fd[1], "\0", 1);
	}
	else
	{
		close(pipex->fd[1]);
		close(pipex->fd[0]);
		return (-1);
	}
	return (0);
}
