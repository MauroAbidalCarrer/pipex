/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 13:50:20 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/03 20:56:25 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <unistd.h>
# include <stdlib.h>
#include <fcntl.h>
# include <stdio.h>
# ifndef IN
#  define IN 0
# endif
# ifndef OUT
#  define OUT 1
# endif
# ifndef CREAT_MODE
#  define CREAT_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# endif

typedef struct cmd
{
	char	*path;
	char	**av;
	char	to_free;
	char	not_found;
	char	dont_print_msg;
}	t_cmd;
void	exit_with_error(t_cmd * cmd);
void	get_cmd_path(t_cmd *cmd, char **env);
int	index_of(char c, char *str, int or_get_len);
int	special_case(t_cmd *cmd);
int	try_get_env_var_vals(char *key, char **dst, char **env);
int	sub_cat(char *str, int sub_len, char *str2, char **dst);
#endif
