/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:11 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/03 21:15:43 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
//file names cannot contain '/'
//so it is safe to assume that if the cmd contains '/', it is an absolute path
void	get_cmd_path(t_cmd *cmd, char **env)
{
	char	*path;//not mallocked
	char	*end_path;

	if (!try_get_env_var_vals("PATH", &path, env))
	{
		if (sub_cat("/", 1, *cmd->av, &end_path))
			exit_with_error(cmd);
		while (*path)
		{
			if (sub_cat(path, index_of(':', path, 1), end_path, &cmd->path))
				exit_with_error(cmd);
			if (access(cmd->path, X_OK) == 0)
			{
				free(end_path);
				return ;
			}
			free(cmd->path);
			cmd->path = NULL;
			path += index_of(':', path, 1);
			path += *path == ':';
		}
	}
	cmd->not_found = 1;
	exit_with_error(cmd);
}

void	setup_cmd_av(char *cmd_s, t_cmd *cmd, char **env)
{
	int	i;

	i = 0;
	while (cmd_s[i])
	{
		i += index_of(' ', cmd_s + i, 1);
		i += cmd_s[i] == ' ';
		cmd->to_free++;
	}
	cmd->av = malloc(sizeof(char *) * (cmd->to_free + 1));
	if (cmd->av == NULL)
		exit_with_error(NULL);
	cmd->av[cmd->to_free] = 0;
	cmd->to_free = 0;
	while (*cmd_s)
	{
		while (*cmd_s == ' ')
			cmd_s++;
		if (sub_cat(cmd_s, index_of(' ', cmd_s, 1), 0, cmd->av + cmd->to_free))
			exit_with_error(cmd);
		cmd_s += index_of(' ', cmd_s, 1);
		cmd_s += *cmd_s == ' ';
		cmd->to_free += cmd->av[cmd->to_free] != NULL;
	}
}

void	exe_cmd1(char *cmd_s, char *file, char **env, int p_fds[2])
{
	int	fd;
	t_cmd	cmd;
	int	i;

	i = 0;
	while (i < sizeof(t_cmd))
		((unsigned char *)&cmd)[i++] = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1 || dup2(p_fds[1], OUT) == -1)
		exit_with_error(NULL);
	if (close(p_fds[0]) == -1 || dup2(fd, IN) == -1)
		exit_with_error(NULL);
	setup_cmd_av(cmd_s, &cmd, env);
	if (!special_case(&cmd))
		get_cmd_path(&cmd, env);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd);
}

void	exe_cmd2(char *cmd_s, char *file, char **env, int p_fds[2])
{
	int	fd;
	t_cmd	cmd;
	int	i;

	i = 0;
	while (i < sizeof(t_cmd))
		((unsigned char *)&cmd)[i++] = 0;
	setup_cmd_av(cmd_s, &cmd, env);
	if (!special_case(&cmd))
		get_cmd_path(&cmd, env);
	fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, CREAT_MODE);
	if (fd == -1 || dup2(p_fds[0], IN) == -1)
		exit_with_error(&cmd);
	if (close(p_fds[1]) == -1 || dup2(fd, OUT) == -1)
		exit_with_error(&cmd);
	execve(cmd.path, cmd.av, env);
	exit_with_error(&cmd);
}

int	main(int ac, char **av, char **env)
{
	int	p_fds[2];
	int	child_pid;

	if (ac != 5)
	{
		write(2, "pipex: Error: nb of arguments isn't 4\n", 38);
		return (1);
	}
	if (pipe(p_fds) == -1)
		exit_with_error(NULL);
	child_pid = fork();
	if (child_pid == -1)
		exit_with_error(NULL);
	if (child_pid == 0)
		exe_cmd2(av[3], av[4], env, p_fds);
	exe_cmd1(av[1], av[2], env, p_fds);
}
