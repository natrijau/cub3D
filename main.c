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

	img.img = mlx_new_image(data->mlx, data->minimap.width * CASE, data->minimap.width * CASE);
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
	mlx_destroy_image(data->mlx, data->minimap.space.img);/* 
	mlx_destroy_image(data->mlx, data->minimap.raycast.img); */
	mlx_destroy_image(data->mlx, data->minimap.character.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	exit(EXIT_SUCCESS);
}

int	wall_hit(t_inter ray, char c)
{
	if (c == 'h')
		ray.y -= ray.pixel;
	else
		ray.x -= ray.pixel;
	if (ray.x >= ray.data->minimap.width * CASE
		|| ray.y >= ray.data->minimap.height * CASE
		|| ray.x < 0 || ray.y < 0)
		return (1);
	if (ray.data->map[(int)ray.y / CASE][(int)ray.x / CASE] == ' '
		|| ray.data->map[(int)(ray.y + CASE - 1) / CASE][(int)(ray.x + CASE - 1) / CASE] == ' '
		|| ray.data->map[(int)ray.y / CASE][(int)(ray.x + CASE - 1) / CASE] == ' '
		|| ray.data->map[(int)(ray.y + CASE - 1) / CASE][(int)ray.x / CASE] == ' ')
		return (1);
	return (0);
}

t_inter	get_h_inter(t_inter ray)
{
	ray.y_step = CASE;
	ray.x_step = CASE / tan(ray.angle);
	ray.y = floor(ray.data->x / CASE) * CASE;
	ray.pixel = 1;
	if (ray.angle > M_PI && ray.angle < 2 * M_PI)
	{
		ray.y += CASE;
		ray.pixel = -1;
	}
	else
		ray.y_step *= -1;
	ray.x = ray.data->x + (ray.y - ray.data->y) / tan(ray.angle);
	if ((ray.angle > (M_PI / 2) && ray.angle < (3 * M_PI) / 2
		&& ray.x_step > 0) || (ray.x_step < 0
		&& !(ray.angle > (M_PI / 2) && ray.angle < (3 * M_PI) / 2)))
		ray.x_step *= -1;
	while (!wall_hit(ray, 'h'))
	{
		if (!(ray.x < 0 || ray.y < 0 || ray.x > ray.data->minimap.width * CASE || ray.y > ray.data->minimap.height * CASE))
			ft_mlx_pixel_put(&ray.data->minimap.space, ray.x, ray.y, 0x0000FF00);
		ray.x += ray.x_step;
		ray.y += ray.y_step;
	}
	ray.distance = sqrt(pow(ray.x - ray.data->x, 2) + pow(ray.y - ray.data->y, 2));
	return (ray);
}

t_inter	get_v_inter(t_inter ray)
{
	ray.x_step = CASE;
	ray.y_step = CASE * tan(ray.angle);
	ray.x = (ray.data->x / CASE) * CASE;
	ray.y_multi = 1;
	if (ray.angle > (M_PI / 2) && ray.angle < (3 * M_PI) / 2)
	{
		ray.x += CASE;
		ray.pixel = -1;
	}
	else
		ray.x_step *= -1;
	ray.y = ray.data->y + (ray.x - ray.data->x) * tan(ray.angle);
	if ((ray.angle > M_PI && ray.angle < 2 * M_PI
		&& ray.y_step < 0) || (ray.y_step > 0
		&& !(ray.angle > M_PI && ray.angle < 2 * M_PI)))
		ray.y_step *= -1;
	while (!wall_hit(ray, 'v'))
	{
		if (!(ray.x < 0 || ray.y < 0 || ray.x > ray.data->minimap.width * CASE || ray.y > ray.data->minimap.height * CASE))
			ft_mlx_pixel_put(&ray.data->minimap.space, ray.x, ray.y, 0x0000FF00);
		ray.x += ray.x_step;
		ray.y += ray.y_step;
	}
	ray.distance = sqrt(pow(ray.x - ray.data->x, 2) + pow(ray.y - ray.data->y, 2));
	return (ray);
}

void	fill_img(t_image *img, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < img->line_len)
	{
		j = 0;
		while (j < img->bpp / 8)
		{
			ft_mlx_pixel_put(img, i, j, color);
			++j;
		}
		++i;
	}
}
// PAS DUTOUT FONCTIONNEL !!!
void	draw_ray(t_data *data, t_inter ray)
{
	int		x;
	int		y;
	int		i;
	int		x_multi;
	int		y_multi;

	x_multi = 1;
	if (ray.x < data->x)
		x_multi = -1;
	y_multi = 1;
	if (ray.y < data->y)
		y_multi = -1;
	x = data->x;
	y = data->y;
	ray.x_step = (x_multi * -1 * data->x + ray.x * x_multi);
	ray.y_step = (y_multi * -1 * data->y + ray.y * y_multi);
	printf("step: x = %f, y = %f\n", ray.x_step, ray.y_step);
	i = 0;
	while (i < (ray.x_step * ray.y_step))
	{
		if (x != ray.x)
        	x += ray.x_step / (ray.x_step * ray.y_step);
		if (y != ray.y)
        	y += ray.y_step / (ray.x_step * ray.y_step);
		if (x < 0 || y < 0 || x > data->minimap.width * CASE || y > data->minimap.height * CASE)
			return ;
		ft_mlx_pixel_put(&data->minimap.space, x, y, 0x0000FF00);/* 
		printf("incr: x = %f, y = %f\n", (x_multi * -1 * data->x - ray.x * x_multi) / (y_multi * -1 * data->y - ray.y * y_multi), (y_multi * -1 * data->y - ray.y * y_multi) / (x_multi * -1 * data->x - ray.x * x_multi));
		printf("plyr: x = %d, y = %d\n", data->x, data->y);
		printf("x = %d, y = %d\n", x, y);
		printf("ray: x = %f, y = %f\n", ray.x, ray.y); */
		++i;
	}
}

void	render_raycast(t_data *data)
{/* 
	t_image	img; */
	t_inter	ray;
	t_inter	h_ray;
	t_inter	v_ray;
	int		i_ray;

	/* img.img = mlx_new_image(data->mlx, data->minimap.width * CASE, data->minimap.height * CASE);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	fill_img(&img, 0xFF000000); // ne fonctionne pas (fond transparent) */
	ray.angle = data->angle - (data->fov_rad / 2);
	ray.data = data;
	i_ray = 0;
	while(i_ray < 80)
	{
		h_ray = get_h_inter(ray);
		v_ray = get_v_inter(ray);
		ray = v_ray;
		if (h_ray.distance < v_ray.distance)
			ray = h_ray;
		// draw_ray(data, ray);
		if (!(ray.x < 0 || ray.y < 0 || ray.x > data->minimap.width * CASE || ray.y > data->minimap.height * CASE))
			ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x00FFFFFF);
		++i_ray;
		ray.angle += (data->fov_rad / 80);
		if (ray.angle > 3 * M_PI)
			ray.angle -= 2 * M_PI;
		if (ray.angle < 0)
			ray.angle += 2 * M_PI;
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
	/* data->minimap.raycast =  */render_raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	// mlx_put_image_to_window(data->mlx, data->win, data->minimap.raycast.img, 0, 0);
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
	if (keycode == 65363)
		data->angle -= ROTATE_SPEED;
	if (keycode == 65361)
		data->angle += ROTATE_SPEED;
	if (data->angle > 3 * M_PI)
		data->angle -= 2 * M_PI;
	if (data->angle < 0)
		data->angle += 2 * M_PI;
	if (keycode == 65363 || keycode == 65361)
		moove(data, 0, 0);
	return (0);
}

int	create_minimap(t_data *data)
{
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
	data->minimap.space = init_space(data);
	render_raycast(data);
	data->minimap.character = init_character(data->mlx);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	// mlx_put_image_to_window(data->mlx, data->win, data->minimap.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->y, data->x);
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
