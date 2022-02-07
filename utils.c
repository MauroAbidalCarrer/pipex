/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:19:24 by maabidal          #+#    #+#             */
/*   Updated: 2022/02/07 15:31:51 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

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

void	cat_error_msg(char *src, char *dst)
{
	static int i;
	int	j;

	j = 0;
	while (src[j] && i < 2047)
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	dst[i] = 0;
}

int	is_str(char *str1, char *str2)
{
	while (*str1 && *str1 == *str2)
	{
		if (!str1[1] && !str2[1])
			return (1);
		str1++;
		str2++;
	}
	return (0);
}

