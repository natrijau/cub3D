/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_raycast_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:57:27 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/04 17:18:59 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	ft_mlx_get_pixel_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp >> 3));
	return (*(unsigned int *)dst);
}

void	set_texture_config(t_data *data, t_ray ray, t_raycast *raycast)
{
	if (data->map[(int)(ray.y - ray.y_step) / CASE][(int)ray.x / CASE] == '0')
	{
		raycast->x = fmod(ray.x, CASE);
		if (ray.y > data->y)
		{
			raycast->actual_wall = raycast->n_wall;
			raycast->x = CASE - raycast->x;
		}
		else
			raycast->actual_wall = raycast->s_wall;
	}
	else
	{
		raycast->x = fmod(ray.y, CASE);
		if (ray.x > data->x)
			raycast->actual_wall = raycast->w_wall;
		else
		{
			raycast->x = CASE - raycast->x;
			raycast->actual_wall = raycast->e_wall;
		}
	}
	if (data->map[(int)ray.y / CASE][(int)ray.x / CASE] == 'D')
		raycast->actual_wall = raycast->door;
	raycast->x *= (double)raycast->actual_wall.width / CASE;
}

int	change_door_case(t_data *data, int x, int y)
{
	int	i;

	if (x == ((int)data->x / CASE) && y == ((int)data->y / CASE))
		return (0);
	i = 0;
	while (data->tab_door[i])
	{
		if (data->tab_door[i][0] == x && data->tab_door[i][1] == y)
		{
			if (data->map[y][x] == 'D')
				data->map[y][x] = '0';
			else
				data->map[y][x] = 'D';
			data->change_state_door = FALSE;
			data->door_frame = TRUE;
			return (1);
		}
		++i;
	}
	return (0);
}
