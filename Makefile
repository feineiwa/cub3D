NAME = cub3d

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

INCS = -I./inc/ -I$(LIBFT) $(LIBMLX)

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -O3

LIBFT = ./libft/

LIBMLX = ./mlx/

B_FLAG = 0

SRC = ./parse/parsing.c ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parse_utils.c ./parse/ft_free_utils.c \
	cub3d.c player.c music.c door.c ray_cast.c video.c ft_free.c load_images.c key_event.c mouse_event.c drawing.c \
	collision.c render_utils.c calcul.c minimap.c init.c move.c sprites.c game_loop.c cast_door.c

OBJS = $(SRC:.c=.o)

RM = rm -rf

all : $(NAME)

%.o : %.c
	@$(CC) -D BONUS=$(B_FLAG) -c $< $(CFLAGS) $(INCS) $(LFLAGS) -o $@

$(NAME) : $(OBJS)
		@make -C $(LIBFT)
		@make -C $(LIBMLX)
		$(CC) $(OBJS) $(LFLAGS) -o $@

bonus :
	$(MAKE) B_FLAG=1 all

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