/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:54:38 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/03 18:51:34 by gdalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_cleanup(t_pipex pipex)
{
	int	i;
	int	j;

	close(pipex.in_fd);
	close(pipex.out_fd);
	if (pipex.here_doc)
		unlink("here_doc.txt");
	i = -1;
	while (pipex.cmd_path[++i])
		free(pipex.cmd_path[i]);
	free(pipex.cmd_path);
	i = -1;
	while (pipex.cmd_args[++i])
	{
		j = -1;
		while (pipex.cmd_args[i][++j])
			free(pipex.cmd_args[i][j]);
		free(pipex.cmd_args[i]);
	}
	free(pipex.cmd_args);
}

void	ft_exec(t_prev prev, t_pipex *pipex, int i, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_error("fork failed");
	if (pid == 0)
	{
		dup2(prev.in, STDIN_FILENO);
		dup2(prev.out, STDOUT_FILENO);
		if (execve(pipex->cmd_path[i], pipex->cmd_args[i], envp) == -1)
			ft_error("execve failed");
	}
}

void	ft_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	ft_loop(t_pipex *pipex, t_prev *prev, char **envp)
{
	while (++prev->i < pipex->cmd_count)
	{
		fprintf(stderr, "%s \n", pipex->cmd_path[prev->i]);
		
		
		if (pipe(pipex->fd) == -1)
			ft_error("pipe failed");
		
		if (prev->i == pipex->cmd_count - 1)
		{
			prev->out = pipex->out_fd;
			ft_exec(*prev, pipex, prev->i, envp);
		}
		else
		{
			prev->out = pipex->fd[1];
			ft_exec(*prev, pipex, prev->i, envp);
		}
		close(pipex->fd[1]);
		if (prev->in != pipex->in_fd)
			close(prev->in);
		prev->in = pipex->fd[0];
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	t_prev	prev;

	ft_init_struct(&pipex, ac, av, envp);
	if (pipex.here_doc)
		ft_here_doc(pipex.in_fd, av[2]);
	prev.in = pipex.in_fd;
	prev.i = -1;
	ft_loop(&pipex, &prev, envp);
	ft_cleanup(pipex);
	return (pipex.exit_code);
}
