NAME	:=	tetris
CFLAGS	:=

SRCS	:=	tetris.c
OBJS	:=	$(SRCS:.c=.o)

RM		:= 	rm -rf

.PHONY:	all clean fclean re

all: $(NAME)

$(NAME):	$(OBJS)
	gcc $(CFLAGS) -lncurses -o $@ $^

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all