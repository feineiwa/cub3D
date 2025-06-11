NAME = cub3d

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

INCS = -I./inc/ -I$(LIBFT) $(LIBMLX)

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -O3

LIBFT = ./libft/

LIBMLX = ./mlx/

SRC = ./parse/parsing.c ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parse_utils.c ./parse/ft_free_utils.c \
	./src/player.c ./src/music.c ./src/door.c ./src/ray_cast.c ./src/video.c ./src/ft_free.c ./src/load_images.c ./src/key_event.c ./src/mouse_event.c ./src/drawing.c \
	./src/collision.c ./src/render_utils.c ./src/calcul.c ./src/minimap.c ./src/init.c ./src/move.c ./src/sprites.c ./src/game_loop.c ./src/cast_door.c

SRCS = $(SRC) ./src/cub3d.c

SRCS_BONUS = $(SRC) ./bonus/cub3d_bonus.c

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

RM = rm -rf

all : $(NAME)

%.o : %.c
	@$(CC) -c $< $(CFLAGS) $(INCS) $(LFLAGS) -o $@

$(NAME) : $(OBJS)
		@make -C $(LIBFT)
		@make -C $(LIBMLX)
		$(CC) $(OBJS) $(LFLAGS) -o $@

bonus: $(OBJS_BONUS)
	@make -C $(LIBFT)
	@make -C $(LIBMLX)
	$(CC) $(OBJS_BONUS) $(LFLAGS) -o cub3d_bonus


clean:
	@make clean -C $(LIBFT)
	$(RM) $(OBJS)
	$(RM) $(OBJS_BONUS)

fclean: clean
	@make fclean -C $(LIBFT)
	@make clean -C $(LIBMLX)
	$(RM) $(NAME) cub3d_bonus

re: fclean all

.PHONY: clean fclean re all run