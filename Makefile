NAME = cub3d

CC = cc

CFLAGS = -g

INCS = -I./inc/ -I$(LIBFT) $(LIBMLX)

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -o3

LIBFT = ./libft/

LIBMLX = ./mlx/

SRC = ./parse/parsing.c ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parse_utils.c ./parse/ft_free_utils.c \
	cub3d.c player.c music.c door.c ray_cast.c video.c ft_free.c

OBJS = $(SRC:.c=.o)

RM = rm -rf

all : $(NAME)

%.o : %.c
	@$(CC) -D BONUS=0 -c $< $(CFLAGS) $(INCS) $(LFLAGS) -o $@

$(NAME) : $(OBJS)
		@make -C $(LIBFT)
		@make -C $(LIBMLX)
		$(CC) $(OBJS) $(LFLAGS) -o $@


clean:
	@make clean -C $(LIBFT)
	$(RM) $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT)
	@make clean -C $(LIBMLX)
	$(RM) $(NAME)

re: fclean all

run: fclean re
	./$(NAME)

.PHONY: clean fclean re all run