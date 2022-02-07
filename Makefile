# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maabidal <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/29 20:08:50 by maabidal          #+#    #+#              #
#    Updated: 2022/02/07 23:29:47 by maabidal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MANDATORY_MAIN	=	main.c

SRCS 		=	cmd_utils.c\
			process.c\
			sys_calls.c\
			utils.c

BONUS_SRCS 	=	get_next_line_bonus.c\
			get_next_line_utils_bonus.c\
			main_bonus.c

NAME	= pipex
CFLAGS	= -Wall -Wextra -Werror
RM	= rm -f
OBJS	= $(SRCS:.c=.o)
BNSOBJS	= $(BONUS_SRCS:.c=.o)
MAINOBJS= $(MANDATORY_MAIN:.c=.o)

all : $(NAME)

bonus : $(BNSOBJS) $(OBJS)
	$(CC) -o $(NAME) $(BNSOBJS) $(OBJS)

$(NAME) : $(OBJS) $(MAINOBJS)
	$(CC) -o $(NAME) $(OBJS) $(MAINOBJS)

.o : .c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./

clean : 
	$(RM) $(OBJS) $(BNSOBJS) $(MAINOBJS)

fclean : clean
	$(RM) $(NAME)

re :    fclean all

.PHONY : all clean fclean re bonus
