/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cub3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:58:49 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 12:25:42 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	creat_image(t_image *img, void *mlx, int width, int height)
{
	img->img = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp,
			&img->line_len, &img->endian);
}

// Function to place a specific color pixel in an image
void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

// Function to initialize the raycasting image (simulated 3D view)
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

// Function to create and display the minimap and its components
int	init_cub3d(t_data *data)
{
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!data->win)
		return (-1);
	data->height = ft_strtablen(data->map) * CASE;
	data->width = ft_strlen(data->map[0]) * CASE;
	data->hook.keyboard_bool = TRUE;
	data->hook.move_forward = FALSE;
	data->hook.move_back = FALSE;
	data->hook.move_right = FALSE;
	data->hook.move_left = FALSE;
	data->hook.rotate_left = FALSE;
	data->hook.rotate_right = FALSE;
	creat_image(&data->img_win, data->mlx, WIDTH, HEIGHT);
	init_img_win(data);
	raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
	return (0);
}
