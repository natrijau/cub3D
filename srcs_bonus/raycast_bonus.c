/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:53:46 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/09 14:28:15 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_wall(t_data *data, t_raycast raycast, int x)
{
	double		factor;
	double		y;
	double		x_pow;

	raycast.distance = (CASE / raycast.distance) * ((WIDTH >> 1)
			/ tan(data->fov_rad / 2));
	factor = (double)raycast.actual_wall.height / raycast.distance;
	y = (HEIGHT >> 1) - raycast.distance / 2;
	if (y <= 0)
		y = -1;
	raycast.y = (y - (HEIGHT >> 1) + raycast.distance / 2) * factor;
	if (raycast.y < 0)
		raycast.y = 0;
	x_pow = pow(x + 1 - (WIDTH - data->calculs.h_div_ten), 2);
	while (++y < (HEIGHT >> 1) + raycast.distance / 2 && y <= HEIGHT)
	{
		raycast.wall_color = ft_mlx_get_pixel_color(&raycast.actual_wall,
				raycast.x, raycast.y);
		if ((x < data->calculs.minimap_pos_x && y < data->calculs.minimap_pos_y)
			|| sqrt(x_pow + pow(y + 1 - (HEIGHT - data->calculs.h_div_ten), 2))
			> data->calculs.h_div_ten + 2)
			ft_mlx_pixel_put(&data->img_win, x, y, raycast.wall_color);
		raycast.y += factor;
	}
}

void	steps_progression(t_data *data, t_ray *ray, int *check_wall)
{
	while (data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& (data->map[(int)(ray->y - ray->y_step) / CASE]
		[(int)ray->x / CASE] == '0' || data->map[(int)ray->y / CASE]
		[(int)(ray->x - ray->x_step) / CASE] == '0')
		&& data->raycast.distance < 50 * CASE)
	{
		if (data->raycast.distance <= data->calculs.h_div_ten)
			ft_mlx_pixel_put(&data->img_win, data->calculs.ray_pos_x
				+ (ray->x - data->x), data->calculs.ray_pos_y
				+ (ray->y - data->y), 0x00FFFFFF);
		ray->x += ray->x_step;
		ray->y += ray->y_step;
		data->raycast.distance = sqrt(pow(data->x - ray->x, 2)
				+ pow(data->y - ray->y, 2));
		if (*check_wall && data->raycast.distance <= CASE
			&& change_door_case(data, (int)ray->x / CASE, (int)ray->y / CASE))
			*check_wall = FALSE;
	}
}

void	raycast_projection(t_data *data, t_ray *ray, int i_ray, int check_wall)
{
	if (i_ray != WIDTH >> 1 || !data->change_state_door)
		check_wall = FALSE;
	data->raycast.distance = sqrt(pow(data->x - ray->x, 2)
			+ pow(data->y - ray->y, 2));
	steps_progression(data, ray, &check_wall);
	if ((fabs(ray->x_step) > 0.01 || fabs(ray->y_step) > 0.01)
		&& data->raycast.distance < 50 * CASE)
	{
		ray->x -= ray->x_step;
		ray->y -= ray->y_step;
		ray->x_step *= 0.1;
		ray->y_step *= 0.1;
		raycast_projection(data, ray, i_ray, check_wall);
	}
}

void	raycast(t_data *data)
{
	t_ray	ray;
	int		i_ray;

	ray.angle = data->angle - data->first_rayangle;
	i_ray = 0;
	while (i_ray < WIDTH)
	{
		ray.angle = fmod(ray.angle, data->calculs.north);
		ray.x_step = (cos(ray.angle + M_PI)
				+ cos(ray.angle + data->calculs.east)) * 0.1;
		ray.y_step = (sin(ray.angle + M_PI)
				+ sin(ray.angle + data->calculs.east)) * 0.1;
		ray.x = data->x + ray.x_step;
		ray.y = data->y + ray.y_step;
		raycast_projection(data, &ray, i_ray, TRUE);
		if (data->raycast.distance < 50 * CASE)
		{
			data->raycast.distance *= cos(fmod(ray.angle
						- (data->angle + (M_PI / 4)), data->calculs.north));
			set_texture_config(data, ray, &data->raycast);
			draw_wall(data, data->raycast, i_ray);
		}
		++i_ray;
		ray.angle += data->angle_step;
	}
}
