/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:42:37 by gdalmass          #+#    #+#             */
/*   Updated: 2024/12/03 18:44:42 by gdalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_get_cmd_path(char **arr, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;

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
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path);
}

void	ft_init_first(t_pipex *pipex, int ac, char **av)
{
	int		fd;

	pipex->here_doc = !ft_strncmp(av[1], "here_doc", 8);
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
	pipex->exit_code = 0;

}

void	ft_init_second(t_pipex *pipex, int ac, char **av, char **envp)
{
	int		i;
	char	**path_arr;

	pipex->cmd_count = ac - (3 + pipex->here_doc);
	path_arr = ft_split(ft_get_path_env(envp), ':');
	pipex->cmd_args = malloc((pipex->cmd_count + 1) * sizeof(char **));
	pipex->cmd_path = malloc((pipex->cmd_count + 1) * sizeof(char *));
	pipex->cmd_args[pipex->cmd_count] = NULL;
	pipex->cmd_path[pipex->cmd_count] = NULL;
	i = -1;
	while (++i < (pipex->cmd_count))
	{
		pipex->cmd_args[i] = ft_custom_split(av[i + 2 + pipex->here_doc], 32, pipex);
		pipex->cmd_path[i] = ft_get_cmd_path(path_arr, pipex->cmd_args[i][0]);
	}
	i = -1;
	while (path_arr[++i])
		free(path_arr[i]);
	free(path_arr);
}

void	ft_init_struct(t_pipex *pipex, int ac, char **av, char **envp)
{
	ft_init_first(pipex, ac, av);
	ft_init_second(pipex, ac, av, envp);
}