NAME	=	tetris

CFLAGS	= 

SRCS_DIR = ./srcs/

SRCS	=	tetris.c \
		init_game.c \
		game_loop.c \
		finish_game.c \
		common.c \
		shape.c \
		display.c \
		time.c

OBJS	=	$(SRCS:.c=.o)

HEADER_DIR = ./includes/

HEADER = set_headers.h \
		tetris.h \
		types.h

RM		= 	rm -rf

all: $(NAME)

$(NAME):	-I${HEADER_DIR}${HEADER} ${SRCS_DIR}$(OBJS)
	gcc $(CFLAGS) ${HEADER_DIR}${HEADER} ${SRCS_DIR}$(OBJS) -o ${NAME}
#-lncurses -o $@ $^

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:	all clean fclean re