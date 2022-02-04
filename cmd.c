/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 14:33:52 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/04 20:51:00 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"


int	has_access(char *cmd_path, int free_path, t_cmd *cmd)
{
	if (access(cmd_path, F_OK) == 0)
	{
		if (access(cmd_path, X_OK) == 0)
			return (1);
		//cmd exists but cannot be executed
		if (free_path)
			free(cmd_path);
		exit_wit_error(cmd, *cmd->av, 126);
	}
	return (0);
}

//file names cannot contain '/'
//so it is safe to assume that if the cmd contains '/', it is an absolute path
//+ 1, car si '/' est a i=0 ca fait += 0
//when '' is called, zsh returns permission denied
//but if the the quotes qre full of spaces, it returns command not found idk why
int	is_special_case(t_cmd *cmd, char *cmd_s)
{
	if (!cmd->ac)
	{
		if (!*cmd_s)
			write(2, "zsh: permission denied: \n", 25);
		else
		{
			write(2, "zsh: command not found: ", 24);
			write(2, cmd_s, index_of(0, cmd_s, 0));
		}
		cmd->dont_print_msg = 1;
		exit_with_error(cmd, NULL, 126 + (*cmd_s != 0));
	}
	if (index_of('/', *cmd->av, 0) != -1)
	{
		if (has_access(*cmd->av, 0, cmd))
		{
			cmd->path = *cmd->av;
			while (index_of('/', *cmd->av, 0) != -1)
				*cmd->av += index_of('/', *cmd->av, 0) + 1;
			return (1);
		}
		exit_with_error(cmd, );
	}
	return (0);
}

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
			if (has_access(cmd->path, X_OK) == 0)
				return (free(end_path));
			free(cmd->path);
			cmd->path = NULL;
			path += index_of(':', path, 1);
			path += *path == ':';
		}
		free(end_path);
	}
	cmd->not_found = 1;
	exit_with_error(cmd, NULL, 127);
}

void	setup_cmd_av(char *cmd_s, t_cmd *cmd, char **env)
{
	int	i;

	i = 0;
	while (cmd_s[i])
	{
		i += index_of(' ', cmd_s + i, 1);
		i += cmd_s[i] == ' ';
		cmd->ac++;
	}
	cmd->av = malloc(sizeof(char *) * (cmd->ac + 1));
	if (cmd->av == NULL)
		exit_with_error(NULL);
	cmd->av[cmd->ac] = 0;
	cmd->ac = 0;
	while (*cmd_s)
	{
		while (*cmd_s == ' ')
			cmd_s++;
		if (sub_cat(cmd_s, index_of(' ', cmd_s, 1), 0, cmd->av + cmd->ac))
			exit_with_error(cmd);
		cmd_s += index_of(' ', cmd_s, 1);
		cmd_s += *cmd_s == ' ';
		cmd->ac += cmd->av[cmd->ac] != NULL;
	}
}

void	setup_cmd(t_cmd *cmd, char *cmd_s)
{
	int	i;

	i = 0;
	while (i < sizeof(t_cmd))
		((unsigned char *)cmd)[i++] = 0;
	setup_cmd_av(cmd_s, &cmd, env);
	if (!special_case(&cmd))
		get_cmd_path(&cmd, env);
}
