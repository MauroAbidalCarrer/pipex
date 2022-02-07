# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maabidal <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/29 20:08:50 by maabidal          #+#    #+#              #
#    Updated: 2022/02/07 22:56:58 by maabidal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS 		=	cmd_utils.c\
				main.c\
				process.c\
				sys_calls.c\
				utils.c

BONUS_SRCS 	=	get_next_line_bonus.c\
				get_next_line_bonus.h\
				get_next_line_utils_bonus.c\
				main_bonus.c

NAME	= pipex
CFLAGS	= -Wall -Wextra -Werror
RM	= rm -f
OBJS	= $(SRCS:.c=.o)
BNSOBJS	= $(BONUS_SRCS:.c=.o)

all : $(NAME)

bonus : $(BNSOBJS)
	$(CC) -o $(NAME) $(BNSOBJS)

$(NAME) : $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

.o : .c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./

clean : 
	$(RM) $(OBJS) $(BNSOBJS)

fclean : clean
	$(RM) $(NAME)
	$(RM) $(BNS_NAME)

re :    fclean all

.PHONY : all clean fclean re bonus
