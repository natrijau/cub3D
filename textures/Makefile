NAME=cub3D
NAME_BONUS=cub3D_bonus
CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3 -g
SRCS_FOLDER=srcs/
SRCS_BONUS_FOLDER=srcs_bonus/
SRCS=main.c parsing.c raycast.c init_cub3d.c init_data.c init_data_utils.c utils.c utils_map.c utils_shift.c map.c
SRCS_BONUS=main_bonus.c parsing_bonus.c raycast_bonus.c minimap_bonus.c init_cub3d_bonus.c init_data_bonus.c init_data_utils_bonus.c \
	utils_bonus.c utils_map_bonus.c utils_raycast_bonus.c utils_shift_bonus.c map_bonus.c door_utils_bonus.c
OBJS_FOLDER=objs/
OBJS=$(addprefix $(OBJS_FOLDER), $(SRCS:.c=.o))
OBJS_BONUS=$(addprefix $(OBJS_FOLDER), $(SRCS_BONUS:.c=.o))
HEADERS=srcs/cub3d.h libft/libft.h minilibx-linux/mlx.h minilibx-linux/mlx_int.h
HEADERS_BONUS=srcs_bonus/cub3d_bonus.h libft/libft.h minilibx-linux/mlx.h minilibx-linux/mlx_int.h
MLX=minilibx-linux/libmlx.a
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lXext -lX11 -lm -lz -o $@
	@echo "\n\nCub3D builded"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(MLX) $(LIBFT) $(OBJS_BONUS)
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) $(MLX) -lXext -lX11 -lm -lz -o $@
	@echo "\n\nCub3D bonus builded"

# Règle de compilation des .o dans objs/ pour la version principale
$(OBJS_FOLDER)%.o: $(SRCS_FOLDER)%.c $(HEADERS)
	@mkdir -p $(OBJS_FOLDER)
	@$(CC) $(CFLAGS) -c $< -o $@ -I .
	@echo "Compiling $<..."

# Règle de compilation des .o dans objs/ pour la version bonus
$(OBJS_FOLDER)%.o: $(SRCS_BONUS_FOLDER)%.c $(HEADERS_BONUS)
	@mkdir -p $(OBJS_FOLDER)
	@$(CC) $(CFLAGS) -c $< -o $@ -I .
	@echo "Compiling $<..."

clean:
	@rm -f $(OBJS) $(OBJS_BONUS)map.cub

fclean: clean
	@rm -f $(NAME) $(NAME)_bonus
	@$(MAKE) -s -C libft/ fclean
	@echo "Executable removed"

$(LIBFT):
	@$(MAKE) -s -C libft/ all

re: fclean all

rebonus: fclean bonus

.PHONY: all bonus fclean clean re rebonus
