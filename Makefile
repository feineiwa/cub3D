NAME = cub3d

BONUS = cub3d_bonus

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

INCS = -I./inc/ -I$(LIBFT) $(LIBMLX)

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -O3

LIBFT = ./libft/

LIBMLX = ./mlx/

SRC = ./parse/parsing.c ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parse_utils.c ./parse/ft_free_utils.c \
	./src/player.c ./src/music.c ./src/door.c ./src/ray_cast.c ./src/video.c ./src/ft_free.c ./src/load_images.c ./src/key_event.c ./src/mouse_event.c ./src/drawing.c \
	./src/collision.c ./src/calcul.c ./src/minimap.c ./src/init.c ./src/move.c ./src/sprites.c ./src/game_loop.c ./src/cast_door.c \
	./src/sprites_utils1.c ./src/sprites_utils2.c ./src/ft_free_utils.c ./src/ray_cast_utils.c ./src/door_utils1.c ./src/door_utils2.c ./src/ray_cast_images.c ./src/render.c \
	src/move_extra.c ./src/render_utils1.c ./src/render_utils2.c  ./src/cast_door_utils1.c ./src/cast_door_utils2.c ./src/drawing_utils1.c ./src/drawing_utils2.c

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

bonus: $(BONUS)

$(BONUS) : $(OBJS_BONUS)
	@make -C $(LIBFT)
	@make -C $(LIBMLX)
	$(CC) $(OBJS_BONUS) $(LFLAGS) -o $(BONUS)


clean:
	@make clean -C $(LIBFT)
	$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	@make fclean -C $(LIBFT)
	@make clean -C $(LIBMLX)
	$(RM) $(NAME) $(BONUS)

re: fclean all

.PHONY: clean fclean re all run bonus