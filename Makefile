NAME	=	philo

CC		=	cc
CFLAGS	=	-Wall -Werror -Wextra -g

SRC	=	philosophers.c parse.c utils.c error.c time.c threads.c routine.c
OBJ	=	$(SRC:.c=.o)

all		:	$(NAME)

$(OBJ)	:	$(SRC)
	$(CC) $(CFLAGS) -c $^

$(NAME)	:	$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

clean	:
	rm -f $(OBJ)

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all

clear	:	all clean

.PHONY	:	all clean fclean re clear