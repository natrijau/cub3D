/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/10/08 10:04:37 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_map(char **map, int erase_bool)
{
	int	char_count = 0;
	int	line_count = 0;

	for (int i = 0; map[i]; i++) {
		char_count += printf("%s\n", map[i]);
		line_count++;
	}
	usleep(20000);
	if (!erase_bool)
		return ;
	for (int i = 0; i < line_count; i++) {
		printf("\033[A");
	}
	for (int i = 0; i < char_count; i++) {
		printf("\b \b");
	}
}

int	cub_close(t_data *data)
{
	mlx_clear_window(data->mlx, data->win);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	mlx_destroy_image(data->mlx, data->minimap.character.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	exit(EXIT_SUCCESS);
}

void	moove(t_data *data, int y, int x)
{
	int new_x;
	int new_y;

	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + W) * MOOVE_SPEED) * y;
	new_y += (sin(data->angle + W) * MOOVE_SPEED) * y;
	data->x += new_x;
	data->y += new_y;
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	data->minimap.space = init_space(data);
	ray_cast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - CASE / 2, data->y - CASE / 2);
}

int	key_hook(int keycode, t_data *data)
{
	static int azerty;

	if (keycode == 114)
	{
		if (azerty == TRUE)
		{
			azerty = FALSE;
			printf("Keyboard config set to QWERTY\n");
		}
		else
		{
			azerty = TRUE;
			printf("Keyboard config set to AZERTY\n");
		}
	}
	// ESKAPE
	if (keycode == 65307)
		cub_close(data);

	if (azerty == FALSE)
	{
		// WASD
		if (keycode == 119)
			moove(data, -1, 0);
		if (keycode == 115)
			moove(data, 1, 0);
		if (keycode == 100)
			moove(data, 0, 1);
		if (keycode == 97)
			moove(data, 0, -1);
	}
	else
	{
		// ZQSD
		if (keycode == 122)
			moove(data, -1, 0); // z
		if (keycode == 115)
			moove(data, 1, 0); // s
		if (keycode == 100)
			moove(data, 0, 1); // d
		if (keycode == 113)
			moove(data, 0, -1); // q
	}
	// VISION
	if (keycode == 65361)
		data->angle -= ROTATE_SPEED;
	if (keycode == 65363)
		data->angle += ROTATE_SPEED;
	data->angle = fmod(data->angle, N);
	if (keycode == 65361 || keycode == 65363)
		moove(data, 0, 0);
	return (0);
}

int	init_cub3d(t_data *data)
{
	int height;
	int width;

	height = ft_strtablen(data->map) * CASE;
	width = ft_strlen(data->map[0]) * CASE;

	data->mlx = mlx_init();
	if (!data->mlx)
		return (-1);
	data->win = mlx_new_window(data->mlx, width, height, "Cub3D");
	if (!data->win)
		return (-1);
	return (0);
}

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)
	{
		printf("Error\n");
		return (1);
	}
	if (parsing(&data, av[1]) == -1)
		return (1);
	print_map(data.map, FALSE);
	if (init_cub3d(&data) == -1
		|| create_minimap(&data) == -1)
		return (1);
	mlx_hook(data.win, 17, 4, cub_close, &data);
	mlx_hook(data.win, 2, 1L<<0, key_hook, &data);
	mlx_loop(data.mlx);
	cub_close(&data);
	return (0);
}
