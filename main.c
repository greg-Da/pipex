/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdalmass <gdalmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:41:27 by gdalmass          #+#    #+#             */
/*   Updated: 2024/11/28 12:51:29 by gdalmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_get_cmd_path(char **arr, int index)
{
	int i;
	char *tmp;

	i = 0;
	while (arr[i])
	{
		tmp = ft_strjoin(arr[i], index);
		if (access(tmp, X_OK) == -1)
		{
			free(tmp);
			i++;
		}
		else
			return (tmp);
	}
	//to do : ADD ERROR command not found:
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
	int	fd;
	char	**path_arr;

	pipex->here_doc = ft_strncmp(av[1], "here_doc", 8) == 0 ? 1 : 0;
	fd = open(av[1 + pipex->here_doc], O_RDONLY);
	pipex->in_fd = fd;
	if (fd == -1)
		pipex->is_invalid_infile = 1;
	else
		pipex->is_invalid_infile = 0;
	fd = open(av[ac - 1], O_WRONLY | O_CREAT, 644);
	pipex->out_fd = fd;
	path_arr = ft_split(ft_get_path_env(envp), ':');
	pipex->cmd_path = malloc(2 * sizeof(char *));
	pipex->cmd_path[0] = ft_get_cmd_path(path_arr, av[1 + pipex->here_doc]);
	pipex->cmd_path[1] = ft_get_cmd_path(path_arr, av[ac - 1]);
	pipex->cmd_args = malloc(2 * sizeof(char **));
	pipex->cmd_args[0] = ft_split(av[2 + pipex->here_doc], 32);
	pipex->cmd_args[1] = ft_split(av[3 + pipex->here_doc], 32);
	free(path_arr);
}

int main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	ft_init_struct(&pipex, ac, av, envp);
	
	

	

	// i = 0;
	// while (path_arr[i])
	// {
	// 	free(path_arr[i]);
	// 	i++;
	// }
	// free(path_arr);
	// free(tmp);

	
	
	// char *args[3];
	// args[0] = "ls";
	// args[1] = "-la";
	// args[2] = NULL;
	// execve("/bin/ls", args, envp);
	//printf("This line will not be executed.\n");

	return (0);
}