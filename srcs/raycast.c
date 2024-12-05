/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:59:50 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 12:26:24 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	ft_mlx_get_pixel_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp >> 3));
	return (*(unsigned int *)dst);
}

void	set_texture_config(t_data *data, t_ray ray, t_raycast *raycast)
{
	if (data->map[(int)((ray.y - ray.y_step) / CASE)]
		[(int)(ray.x / CASE)] == '0')
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
	raycast->x *= (double)raycast->actual_wall.width / CASE;
}

void	draw_wall(t_data *data, t_ray ray, int x)
{
	t_raycast	raycast;
	double		factor;
	double		y;

	raycast = data->raycast;
	raycast.distance *= cos(fmod(ray.angle
				- (data->angle + (M_PI / 4)), data->calculs.north));
	raycast.distance = (CASE / raycast.distance) * ((WIDTH >> 1)
			/ tan(data->fov_rad / 2));
	factor = (double)raycast.actual_wall.height / raycast.distance;
	y = (HEIGHT >> 1) - raycast.distance / 2;
	if (y <= 0)
		y = -1;
	raycast.y = (y - (HEIGHT >> 1) + raycast.distance / 2) * factor;
	if (raycast.y < 0)
		raycast.y = 0;
	while (++y < (HEIGHT >> 1) + raycast.distance / 2 && y <= HEIGHT)
	{
		raycast.wall_color = ft_mlx_get_pixel_color(&raycast.actual_wall,
				raycast.x, raycast.y);
		ft_mlx_pixel_put(&data->img_win, x, y, raycast.wall_color);
		raycast.y += factor;
	}
}

void	raycast_projection(t_data *data, t_ray *ray)
{
	while (data->map[(int)(ray->y / CASE)][(int)(ray->x / CASE)] == '0'
		&& (data->map[(int)((ray->y - ray->y_step) / CASE)]
		[(int)(ray->x / CASE)] == '0' || data->map[(int)(ray->y / CASE)]
		[(int)((ray->x - ray->x_step) / CASE)] == '0'))
	{
		ray->x += ray->x_step;
		ray->y += ray->y_step;
		data->raycast.distance = sqrt(pow(data->x - ray->x, 2)
				+ pow(data->y - ray->y, 2));
		if (data->raycast.distance >= 50 * CASE)
			return ;
	}
	if (fabs(ray->x_step) > 0.001 || fabs(ray->y_step) > 0.001)
	{
		ray->x -= ray->x_step;
		ray->y -= ray->y_step;
		ray->x_step *= 0.1;
		ray->y_step *= 0.1;
		raycast_projection(data, ray);
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
		raycast_projection(data, &ray);
		if (data->raycast.distance < 50 * CASE)
		{
			set_texture_config(data, ray, &data->raycast);
			draw_wall(data, ray, i_ray);
		}
		++i_ray;
		ray.angle += data->angle_step;
	}
}
