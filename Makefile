NAME = cub3d

BONUS = cub3d_bonus

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

INCS = -I./inc/ -I$(LIBFT) $(LIBMLX)

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -O3

LIBFT = ./libft/

LIBMLX = ./mlx/

SRCS = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parsing.c ./parse/validated_input.c ./parse/ft_free_parsing.c \
	./parse/validated_map.c  ./parse/parse_utils1.c  ./parse/parse_utils2.c ./parse/parse_map.c ./parse/validated_color.c \
	./src/player.c ./src/calcul.c ./src/collision.c ./src/drawing_utils1.c ./src/drawing_utils2.c ./src/ft_free_utils.c \
	./src/ft_free.c ./src/game_loop.c ./src/init.c ./src/key_event.c ./src/load_images.c ./src/move.c \
	./src/ray_cast_images.c ./src/ray_cast_utils.c ./src/ray_cast.c ./src/render_utils1.c ./src/render_utils2.c \
	./src/render.c ./src/cub3d.c


BONUS_SRCS = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./bonus/parse/parsing.c ./bonus/parse/validated_input.c ./bonus/parse/ft_free_parsing.c \
	./bonus/parse/validated_map.c  ./bonus/parse/parse_utils1.c  ./bonus/parse/parse_utils2.c ./bonus/parse/parse_map.c ./bonus/parse/validated_color.c \
	./bonus/src/player_bonus.c ./bonus/src/music_bonus.c ./bonus/src/door_bonus.c ./bonus/src/ray_cast_bonus.c ./bonus/src/video_bonus.c ./bonus/src/ft_free_bonus.c ./bonus/src/load_images_bonus.c ./bonus/src/key_event_bonus.c ./bonus/src/mouse_event_bonus.c ./bonus/src/drawing_bonus.c \
	./bonus/src/collision_bonus.c ./bonus/src/calcul_bonus.c ./bonus/src/minimap_bonus.c ./bonus/src/init_bonus.c ./bonus/src/move_bonus.c ./bonus/src/sprites_bonus.c ./bonus/src/game_loop_bonus.c ./bonus/src/cast_door_bonus.c \
	./bonus/src/sprites_utils1_bonus.c ./bonus/src/sprites_utils2_bonus.c ./bonus/src/ft_free_utils_bonus.c ./bonus/src/ray_cast_utils_bonus.c ./bonus/src/door_utils1_bonus.c ./bonus/src/door_utils2_bonus.c ./bonus/src/ray_cast_images_bonus.c ./bonus/src/render_bonus.c \
	./bonus/src/move_extra_bonus.c ./bonus/src/render_utils1_bonus.c ./bonus/src/render_utils2_bonus.c  ./bonus/src/cast_door_utils1_bonus.c ./bonus/src/cast_door_utils2_bonus.c ./bonus/src/drawing_utils1_bonus.c ./bonus/src/drawing_utils2_bonus.c \
	./bonus/src/cub3d_bonus.c



OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(BONUS_SRCS:.c=.o)

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