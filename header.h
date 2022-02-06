/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:20 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/06 18:41:50 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# ifndef IN
#  define IN 0
# endif
# ifndef OUT
#  define OUT 1
# endif
# ifndef CREAT_M
#  define CREAT_M S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# endif
# ifndef CREAT_F
#  define CREAT_F  O_WRONLY | O_TRUNC | O_CREAT
# endif
# ifndef APPEND_F
#  define APPEND_F  O_WRONLY | O_APPEND | O_CREAT
# endif
# ifndef F_NOT_FOUND_ES
#  define F_NOT_FOUND_ES 2
# endif
# ifndef F_NO_PERM_ES
#  define F_NO_PERM_ES 2
# endif
# ifndef BEF_ERR
#  define BEF_ERR "zsh: "
# endif
# ifndef CMD_NOT_FOUND
#  define CMD_NOT_FOUND "zsh: command not found: "
# endif
# ifndef PERM_DEN
#  define PERM_DEN "zsh: permission denied: "
# endif
/*
# ifndef F_NOT_FOUND
#  define F_NOT_FOUND "zsh: no such file or directory: "
# endif
*/
typedef struct cmd
{
	char	*path;
	char	**av;
	int	ac;
}	t_cmd;
void	exit_with_error(t_cmd *cmd, char *c_msg, char *app_msg, int exit_status);
int	index_of(char c, char *str, int or_get_len);
int	try_get_env_var_vals(char *key, char **dst, char **env);
int	sub_cat(char *str, int sub_len, char *str2, char **dst);
void	setup_cmd(t_cmd *cmd, char *cmd_s, char **env);
void	str_cat(char *src, char *dst);

void	exe_first_cmd(char *cmd_s, char *file, char **env, int p_fds[2]);
void	exe_last_cmd(char *cmd_s, char *file, char **env, int p_fds[2], int O_MODE);
#endif
