#include "cub3d.h"

void	creat_image(t_image *img, void *mlx, int width, int height)
{
	img->img = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp,
			&img->line_len, &img->endian);
}

// Fonction pour placer un pixel de couleur spécifique dans une image
void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
void	init_minimap(t_data *data)
{
	double	distance;
	int		x;
	int		y;

	y = 0;
	while (y < H_DIV_FIVE)
	{
		x = 0;
		while (x < H_DIV_FIVE)
		{
			distance = sqrt(pow(x - H_DIV_TEN, 2) + pow(y - H_DIV_TEN, 2));
			if (distance <= CASE_DIV_TWO)
				ft_mlx_pixel_put(&data->img_win,
					MINIMAP_POS_X + x, MINIMAP_POS_Y + y, 0x00FF0000);
			else if (distance <= H_DIV_TEN)
			{
				if (x + data->x - H_DIV_TEN >= 0
					&& y + data->y - H_DIV_TEN >= 0
					&& x + data->x - H_DIV_TEN < data->width_and_case
					&& y + data->y - H_DIV_TEN < data->height_and_case
					&& data->map[(int)((y + data->y - H_DIV_TEN) / CASE)]
					[(int)((x + data->x - H_DIV_TEN) / CASE)] == '0')
					ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
						MINIMAP_POS_Y + y, 0x00A0A0A0);
				else
					ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
						MINIMAP_POS_Y + y, 0x00000000);
			}
			else if (distance <= H_DIV_TEN + 1)
				ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
					MINIMAP_POS_Y + y, 0x00FFFFFF);
			++x;
		}
		++y;
	}
}

// Fonction pour initialiser l'image du raycasting (vue en 3D simulée)
void	init_img_win(t_data *data)
{
	t_raycast	raycast;
	int			i;
	int			k;

	raycast = data->raycast;
	k = 0;
	while (k < HEIGHT)
	{
		i = 0;
		while (i < WIDTH)
		{
			if (k < HEIGHT / 2)
				ft_mlx_pixel_put(&data->img_win, i, k, raycast.ceiling_color);
			else
				ft_mlx_pixel_put(&data->img_win, i, k, raycast.floor_color);
			++i;
		}
		++k;
	}
}

// Fonction pour créer et afficher la minimap et ses composants
int	init_cub3d(t_data *data)
{
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!data->win)
		return (-1);
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
	data->height_and_case = data->minimap.height * CASE;
	data->width_and_case = data->minimap.width * CASE;
	data->hook.keyboard_bool = FALSE;
	data->hook.move_forward = FALSE;
	data->hook.move_back = FALSE;
	data->hook.move_right = FALSE;
	data->hook.move_left = FALSE;
	data->hook.rotate_left = FALSE;
	data->hook.rotate_right = FALSE;
	data->hook.mouse_move = TRUE;
	data->hook.old_x = WIDTH / 2;
	mlx_mouse_move(data->mlx, data->win, data->hook.old_x, HEIGHT / 2);
	creat_image(&data->img_win, data->mlx, WIDTH, HEIGHT);
	init_img_win(data);
	init_minimap(data);
	ray_cast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
	return (0);
}
