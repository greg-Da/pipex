/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: greg <greg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:41:27 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/02 12:34:58 by greg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *ft_get_cmd_path(char **arr, char *cmd)
{
    int     i;
    char    *path;
    char    *tmp;

    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));

    i = 0;
    while (arr[i])
    {
        tmp = ft_strjoin("/", cmd);
        path = ft_strjoin(arr[i], tmp);
        free(tmp);
        if (access(path, X_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return (NULL);
}

char	*ft_get_path_env(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while(envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0) 
		{
            path = envp[i] + 5;
            break;
        }
        i++;
	}
	return (path);
}

void	ft_init_struct(t_pipex *pipex, int ac, char **av, char **envp)
{
	int		fd;
	int		i;
	char	**path_arr;

	pipex->here_doc = ft_strncmp(av[1], "here_doc", 8) == 0 ? 1 : 0;
	if (pipex->here_doc)
		fd = open("here_doc.txt", O_RDWR | O_CREAT, 0666);
	else
		fd = open(av[1 + pipex->here_doc], O_RDONLY);
	
	pipex->in_fd = fd;
	if (fd == -1)
		pipex->is_invalid_infile = 1;
	else
		pipex->is_invalid_infile = 0;
	if (pipex->here_doc)
		fd = open(av[ac - 1], O_RDWR | O_APPEND | O_CREAT, 0666);
	else
		fd = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0666);
	pipex->out_fd = fd;
	path_arr = ft_split(ft_get_path_env(envp), ':');
	pipex->cmd_args = malloc((ac - (3 + pipex->here_doc) + 1) * sizeof(char **));
	pipex->cmd_path = malloc((ac - (3 + pipex->here_doc) + 1) * sizeof(char *));
	pipex->cmd_args[ac - (3 + pipex->here_doc)] = NULL;
	pipex->cmd_path[ac - (3 + pipex->here_doc)] = NULL;
	i = -1;
	while (++i < (ac - (3 + pipex->here_doc)))
	{
		pipex->cmd_args[i] = ft_split(av[i + 2 + pipex->here_doc], 32);
		pipex->cmd_path[i] = ft_get_cmd_path(path_arr, pipex->cmd_args[i][0]);
	}
	i = -1;
	while (path_arr[++i])
		free(path_arr[i]);
	free(path_arr);
}

void	ft_cleanup(t_pipex pipex)
{
	int	i;
	int	j;

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

void	ft_exec(int in, int out, t_pipex *pipex, int i, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0) {
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);
		execve(pipex->cmd_path[i], pipex->cmd_args[i], envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
    }

	waitpid(pid, NULL, 0);
}

void	ft_get_here_doc_line(int fd, char *limiter)
{
	int		b_read;
	char	*buf;
	char	*end;

	buf = malloc(1000);
	while (1) {
		write(STDOUT_FILENO, "> ", 2);
		b_read = read(STDIN_FILENO, buf, 999);
		if (b_read == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}

		buf[b_read] = '\0';
		end = ft_strchr(buf, '\n');

		if (end && (end - buf) == ft_strlen(limiter) && ft_strncmp(buf, limiter, (end - buf)) == 0)
            break;
		if (write(fd, buf, b_read) == -1)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	free(buf);
	close(fd);
}

int	ft_here_doc(int fd, char *limiter)
{
	int		pipe_fd[2];
	int		pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
        exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
        exit(EXIT_FAILURE);
	}

	if (pid == 0)
		ft_get_here_doc_line(pipe_fd[1], limiter);
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], fd) == -1) {
            perror("dup2 failure");
            exit(EXIT_FAILURE);
        }
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (pid);

}

int main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	int		i;
	int		fd[2];
	int		prev_in;

	ft_init_struct(&pipex, ac, av, envp);
	if (pipex.here_doc && ft_here_doc(pipex.in_fd, av[2]) == 0)
	{
		ft_cleanup(pipex);
		return (0);
	}
	prev_in = pipex.in_fd;
	
	
	i = -1;
	while (pipex.cmd_path[++i])
	{
		if (pipe(fd) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
		if (i == ac - (3 + pipex.here_doc) - 1) {
            ft_exec(prev_in, pipex.out_fd, &pipex, i, envp);
        } else {
            ft_exec(prev_in, fd[1], &pipex, i, envp);
        }
		close(fd[1]); 
		if (prev_in != pipex.in_fd)
			close(prev_in);
		prev_in = fd[0];
	}

	close(fd[0]);
	close(pipex.in_fd);
	close(pipex.out_fd);

	ft_cleanup(pipex);

	return (0);
}