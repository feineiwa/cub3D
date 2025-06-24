NAME = cub3D

BONUS = cub3D_bonus

CC = gcc

CFLAGS = -Wall -Wextra -Werror

INCS = -I./inc/

LFLAGS = -L$(LIBFT) -lft -L$(LIBMLX) -lmlx -lXext -lX11 -lm -lXfixes -O3

LIBFT = ./libft/

LIBMLX = ./mlx/


SRC_PARSE = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./parse/parsing.c ./parse/validated_input.c ./parse/ft_free_parsing.c \
	./parse/validated_map.c  ./parse/parse_utils1.c  ./parse/parse_utils2.c ./parse/validated_color.c ./parse/optimisation.c 
SRC_FILES = player calcul collision drawing_utils1 drawing_utils2 ft_free_utils \
	ft_free game_loop init key_event load_images move \
	ray_cast_images ray_cast_utils ray_cast render_utils1 render_utils2 \
	render cub3d


BONUS_SRCS_PARSE = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c ./bonus/parse/parsing.c ./bonus/parse/validated_input.c ./bonus/parse/ft_free_parsing.c \
	./bonus/parse/validated_map.c  ./bonus/parse/parse_utils1.c  ./bonus/parse/parse_utils2.c ./bonus/parse/validated_color.c \
	./bonus/parse/optimisation.c

BONUS_FILES = player_bonus music_bonus door_bonus ray_cast_bonus video_bonus ft_free_bonus load_images_bonus key_event_bonus mouse_event_bonus drawing_bonus \
	collision_bonus calcul_bonus minimap_bonus init_bonus move_bonus sprites_bonus game_loop_bonus cast_door_bonus \
	sprites_utils1_bonus sprites_utils2_bonus ft_free_utils_bonus ray_cast_utils_bonus door_utils1_bonus door_utils2_bonus ray_cast_images_bonus render_bonus \
	move_extra_bonus render_utils1_bonus render_utils2_bonus  cast_door_utils1_bonus cast_door_utils2_bonus drawing_utils1_bonus drawing_utils2_bonus \
	cub3d_bonus

#colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

SRCS = $(SRC_PARSE) $(addprefix ./src/, $(addsuffix .c, $(SRC_FILES)))

BONUS_SRCS = $(BONUS_SRCS_PARSE) $(addprefix ./bonus/src/, $(addsuffix .c, $(BONUS_FILES)))

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(BONUS_SRCS:.c=.o)

RM = rm -rf

all : cmlx $(NAME)

%.o : %.c
	@echo "$(CYAN)compiling: $< $(DEF_COLOR)"
	@$(CC) -c $< $(CFLAGS) $(INCS) $(LFLAGS) -o $@

$(NAME) : $(OBJS)
		@make -C $(LIBFT)
		@make -C $(LIBMLX)
		@$(CC) $(OBJS) $(LFLAGS) -o $@

bonus: cmlx $(BONUS)

$(BONUS) : $(OBJS_BONUS)
	@make -C $(LIBFT)
	@make -C $(LIBMLX)
	@$(CC) $(OBJS_BONUS) $(LFLAGS) -o $(BONUS)
	@echo "$(GREEN)GET READY TO PLAY THE GAME $@ $(DEF_COLOR)"
	@[ -f ./disaster_war.txt ] && cat ./disaster_war.txt || true

cmlx:
	@if [ ! -f "$(LIBMLX)/libmlx.a" ]; then \
		echo "$(MAGENTA)Cloning mlx repository...$(DEF_COLOR)"; \
		git clone https://github.com/feineiwa/mlx_without_leaks.git $(LIBMLX); \
	else \
		echo "$(BLUE)libmlx.a found, skipping clone. Nothing to be done.$(DEF_COLOR)"; \
	fi

clean:
	@make clean -C $(LIBFT)
	$(RM) $(OBJS) $(OBJS_BONUS)

fclean: clean
	@make fclean -C $(LIBFT)
	@if [ -d "$(LIBMLX)" ]; then make clean -C $(LIBMLX); fi
	$(RM) $(NAME) $(BONUS)
	@rm -rf "$(LIBMLX)"

re: fclean all

.PHONY: clean fclean re all run bonus cmlx
