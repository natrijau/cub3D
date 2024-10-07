/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/10/04 15:17:03 by yanolive         ###   ########.fr       */
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

void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

t_image	init_space(t_data *data)
{
	t_image	img;
	int		color;
	int		x, y, i, j;

	img.img = mlx_new_image(data->mlx, data->minimap.width * CASE, data->minimap.height * CASE);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	color = 0x00FFFFFF;
	y = -1;
	while (++y < data->minimap.height)
	{
		x = -1;
		while (++x < data->minimap.width && data->map[y][x])
		{
			if (data->map[y][x] == '0')
			{
				i = -1;
				while (++i < CASE)
				{
					j = -1;
					while (++j < CASE)
						ft_mlx_pixel_put(&img, x * CASE + i, y * CASE + j, color);
				}
			}
		}
	}
	return (img);
}

t_image	init_character(void *mlx)
{
	t_image	img;
	int		color;
	int		i;
	int		j;

	img.img = mlx_new_image(mlx, CASE, CASE);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	color = 0x00FF0000;
	i = 0;
	while (i < CASE)
	{
		j = 0;
		while (j < CASE)
		{
			ft_mlx_pixel_put(&img, i, j, color);
			++j;
		}
		++i;
	}
	return (img);
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

void	ray_setup(t_ray *ray)
{
	ray->x_multi = -1;
	if (ray->angle > S && ray->angle < N)
		ray->x_multi = 1;
	ray->y_multi = 1;
	if (ray->angle < E && ray->angle > W)
		ray->y_multi = -1;
}

t_ray	get_h_ray(t_data *data, t_ray ray)
{
	ray.x_step = (CASE / tan(ray.angle)) * ray.x_multi;
	ray.y_step = CASE * ray.y_multi;
	ray.y = (data->y / CASE) * CASE;
	if (ray.y_multi == -1)
		ray.y += CASE;
	ray.x = data->x + (ray.y - data->y) / tan(ray.angle);
	while (TRUE)
	{
		if (ray.y < 0 || ray.y > data->minimap.height * CASE
			|| ray.x < 0 || ray.x > data->minimap.width * CASE)
			break ;
		ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);
		if (data->map[(int)(ray.y - ray.y_multi) / CASE][(int)ray.x / CASE] == ' ')
			break ;
		ray.y += ray.y_step;
		ray.x += ray.x_step;
	}
	ray.distance = sqrt(pow(data->x - ray.x, 2) + pow(data->y - ray.y, 2));
	return (ray);
}

t_ray	get_v_ray(t_data *data, t_ray ray)
{
	ray.x_step = CASE * ray.x_multi;
	ray.y_step = (CASE * tan(ray.angle)) * ray.y_multi;
	ray.x = (data->x / CASE) * CASE;
	if (ray.x_multi == -1)
		ray.x += CASE;
	ray.y = data->y + (ray.x - data->x) * tan(ray.angle);
	while (TRUE)
	{
		if (ray.y <= 0 || ray.y >= data->minimap.height * CASE
			|| ray.x <= 0 || ray.x >= data->minimap.width * CASE)
			break ;
		ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);
		if (data->map[(int)ray.y / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' ')
			break ;
		ray.y += ray.y_step;
		ray.x += ray.x_step;
	}
	ray.distance = sqrt(pow(data->x - ray.x, 2) + pow(data->y - ray.y, 2));
	return (ray);
}

void	ray_cast(t_data *data)
{
	t_ray	ray;
	t_ray	h_ray;
	t_ray	v_ray;
	int		i_ray;

	ray.angle = data->angle - data->fov_rad / 2;
	if (ray.angle < 0)
		ray.angle += N;
	ray.map = data->map;
	i_ray = 0;
	while (i_ray < data->minimap.width * CASE)
	{
		ray_setup(&ray);
		h_ray = get_h_ray(data, ray);
		v_ray = get_v_ray(data, ray);
		ray = v_ray;
		if (h_ray.distance < v_ray.distance)
			ray = h_ray;
		// draw_ray_on_minimap(data, ray);
		// draw_wall(data, ray);
		++i_ray;
		ray.angle += data->fov_rad / (data->minimap.width * CASE);
	}
}

void	moove(t_data *data, int y, int x)
{
	int new_x;
	int new_y;

	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + M_PI / 2) * MOOVE_SPEED) * y;
	new_y += (sin(data->angle + M_PI / 2) * MOOVE_SPEED) * y;
	data->x += new_x;
	data->y += new_y;
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	data->minimap.space = init_space(data);
	ray_cast(data); // ne fonctionne pas encore, mais c un debut...
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x, data->y);
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
			moove(data, -1, 0);
		if (keycode == 115)
			moove(data, 1, 0);
		if (keycode == 100)
			moove(data, 0, 1);
		if (keycode == 113)
			moove(data, 0, -1);
	}
	// VISION
	if (keycode == 65361)
		data->angle -= ROTATE_SPEED;
	if (keycode == 65363)
		data->angle += ROTATE_SPEED;
	if (data->angle > N)
		data->angle -= N;
	else if (data->angle < 0)
		data->angle += N;
	/* if (keycode == 65361 || keycode == 65363)
		moove(data, 0, 0); */
	return (0);
}

int	create_minimap(t_data *data)
{
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
	data->minimap.space = init_space(data);
	data->minimap.character = init_character(data->mlx);
	ray_cast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x, data->y);
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
