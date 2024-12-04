/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:54:38 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/04 19:19:23 by gdalmass         ###   ########.fr       */
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

	free(pipex.pids);
	free(pipex.cmd_args);
}

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
		{
			ft_cmd_not_acc(pipex->cmd_args[i][0]);
			write(prev.out, "\0", 1);
			if (prev.i == pipex->cmd_count - 1)
			{
				if (pipex->cmd_path[i] == NULL)
					pipex->exit_code = 127;
				else
					pipex->exit_code = 1;
				exit(1);
			}
		}
		exit(EXIT_SUCCESS);
	}
	pipex->pids_size++;
	pipex->pids = ft_realloc(pipex->pids, (pipex->pids_size) * sizeof(int), (pipex->pids_size + 1) * sizeof(int));
	pipex->pids[pipex->pids_size - 1] = pid;
	// if (prev.i == pipex->cmd_count - 1)
	// {
	// 	waitpid(pid, &status, 0);
	// 	if (WEXITSTATUS(status))
	// 	{
	// 		exit(127);
	// 	}
	// }
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
		if (pipe(pipex->fd) == -1)
			ft_error("pipe failed");
		if (prev->in == -1)
		{
			ft_putstr_fd("pipex: line 1: input: No such file or directory\n", 2);
			if (prev->i == pipex->cmd_count - 1)
				write(pipex->out_fd, "\0", 1);
			else
				write(pipex->fd[1], "\0", 1);
		}
		else
		{
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
	int		i;		// if (pipex.is_invalid_outfile == -1)
		// {
		// 	pipex.exit_code = 1;
		// }
	int		status;

	ft_init_struct(&pipex, ac, av, envp);
	if (pipex.here_doc)
		ft_here_doc(pipex.in_fd, av[2]);
	prev.in = pipex.in_fd;
	prev.i = -1;
	ft_loop(&pipex, &prev, envp);

	i = -1;
	while(pipex.pids_size > ++i)
	{
		waitpid(pipex.pids[i], &status, 0);
		// if (WEXITSTATUS(status) == 1)
		// {
		// 	pipex.exit_code = 1;
		// }
		if (prev.out == -1)
		{
			pipex.exit_code = 1;
		}
		
	}

	ft_cleanup(pipex);
	return (pipex.exit_code);
}
