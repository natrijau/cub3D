/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cub3d_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:49:37 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 12:26:43 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	creat_image(t_image *img, void *mlx, int width, int height)
{
	img->img = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp,
			&img->line_len, &img->endian);
}

void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

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

void	show_animation(t_data *data)
{
	static double	i = 0;

	mlx_put_image_to_window(data->mlx, data->win, data->raycast.fire[(int)i].img, 100, 100);
	i += 0.03;
	if (i > 5)
		i = 0;
}

int	init_cub3d(t_data *data)
{
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!data->win)
		return (-1);
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
	data->height_and_case = data->minimap.height * CASE;
	data->width_and_case = data->minimap.width * CASE;
	data->hook.keyboard_bool = TRUE;
	data->hook.move_forward = FALSE;
	data->hook.move_back = FALSE;
	data->hook.move_right = FALSE;
	data->hook.move_left = FALSE;
	data->hook.rotate_left = FALSE;
	data->hook.rotate_right = FALSE;
	data->hook.old_x = WIDTH / 2;
	mlx_mouse_move(data->mlx, data->win, data->hook.old_x, HEIGHT / 2);
	creat_image(&data->img_win, data->mlx, WIDTH, HEIGHT);
	init_img_win(data);
	init_minimap(data);
	raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
	return (0);
}
