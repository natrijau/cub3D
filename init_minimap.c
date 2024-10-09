#include "cub3d.h"

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

t_image	init_ray_cast(void *mlx)
{
	t_image	img;

	img.img = mlx_new_image(mlx, WIDTH, HEIGHT);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	return (img);
}

int	create_minimap(t_data *data)
{
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
	data->minimap.space = init_space(data);
	data->minimap.character = init_character(data->mlx);
	data->minimap.raycast = init_ray_cast(data->mlx);
	ray_cast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - CASE / 2, data->y - CASE / 2);
	return (0);
}
