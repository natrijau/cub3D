NAME=cub3D
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pg -g # -O3
SRCS=main.c parsing.c ray_cast.c init_cub3d.c init_data.c init_data_utils.c utils.c utils_map.c utils_shift.c utils_bonus.c
OBJS=$(SRCS:.c=.o)
HEADERS=cub3d.h libft/libft.h minilibx-linux/mlx.h minilibx-linux/mlx_int.h
MLX=minilibx-linux/libmlx.a
LIBFT=libft/libft.a

all: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) -lXext -lX11 -lm -lz -o $@
	@echo "\n\nCub3D builded"

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $(HEADERS) $<
	@echo -n '.....'

clean:
	@rm -f $(OBJS)
	@rm -f $(HEADERS:.h=.h.gch)
	@$(MAKE) -s -C libft/ clean
	@echo "All objects clean"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s -C libft/ fclean
	@echo "And the executable too"

$(LIBFT):
	@$(MAKE) -s -C libft/ all

re: fclean all

.PHONY: all fclean clean re