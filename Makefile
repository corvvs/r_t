NAME	:=	tetris
CFLAGS	:=	-Wall -Werror -Wextra

SRCS	:=	tetris.c window.c shape.c time.c init_game.c game_loop.c finish_game.c key_action.c
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
