/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:19:24 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/04 21:03:51 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	exit_with_error(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->not_found)
		{
			write(2, "zsh: command not found: ", 24);
			write(2, *cmd->av, index_of(0, *cmd->av, 0));
			write(2, "\n", 1);
		}
		while (--cmd->ac >= 0)
			free(cmd->av[cmd->ac]);
		free(cmd->av);
		if (cmd->path)
			free(cmd->path);
		if (cmd->not_found)
			exit(127);
	}
	if (!cmd || !cmd->dont_print_msg)
		perror("zsh");
	exit(1);
}

int	try_get_env_var_vals(char *key, char **dst, char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && key[j] && env[i][j] == key[j])
		{
			if (key[j + 1] == 0)
			{
				*dst = env[i] + j + 2;
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	index_of(char c, char *str, int or_get_len)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (i);
	}
	if (or_get_len)
		return index_of(0, str, 0);
	if (c == 0)
		return (i);
	return (-1);
}

//returns 1 if malloc failed
int	sub_cat(char *str, int sub_len, char *str2, char **dst)//demaner a jeremmy parraport au priorites
{
	int	str2_len;

	if (sub_len == 0)
	{
		*dst = NULL;
		return(0);
	}
	str2_len = 0;
	if (str2)
		str2_len = index_of(0, str2, 0);
	*dst = malloc(sizeof(char) * (str2_len + sub_len + 1));
	if (dst == NULL)
		return (1);
	dst[0][str2_len + sub_len] = 0;
	while (str2 && --str2_len >= 0)
		dst[0][sub_len + str2_len] = str2[str2_len];
	while (--sub_len >= 0)
		dst[0][sub_len] = str[sub_len];
	return (0);
}
