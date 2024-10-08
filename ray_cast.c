#include "cub3d.h"

void	set_ray_multi(t_ray *ray)
{
	ray->x_multi = -1;
	if (ray->angle > W && ray->angle < E)
		ray->x_multi = -1;
	ray->y_multi = 1;
	if (!(ray->angle < N && ray->angle > S))
		ray->y_multi = -1;
}

t_ray	get_h_ray(t_data *data, t_ray ray)
{
	ray.x_step = (CASE / tan(ray.angle)) * ray.x_multi;
	if (fabs(ray.angle) < 0.01 || fabs(ray.angle - S) < 0.01)
    	ray.x_step = 0;
	ray.y_step = CASE * ray.y_multi;
	ray.y = (data->y / CASE) * CASE;
	if (ray.y_multi == -1)
		ray.y += CASE;
	ray.x = data->x + (ray.y - data->y) / tan(ray.angle);
	while (TRUE)
	{
		if (ray.y < 0 || ray.y > data->minimap.height * CASE
			|| ray.x < 0 || ray.x > data->minimap.width * CASE
			|| ray.y - ray.y_multi < 0 || ray.x - ray.x_multi < 0
			|| ray.y - ray.y_multi > data->minimap.height * CASE
			|| ray.x - ray.x_multi > data->minimap.width * CASE)
			break ;
		if (data->map[(int)ray.y / CASE][(int)(ray.x) / CASE] == ' '
			|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' '
			|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)ray.x / CASE] == ' '
			|| data->map[(int)ray.y / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' ')
			break ;
		ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);
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
	if (fabs(ray.angle - W) < 0.01 || fabs(ray.angle - E) < 0.01)
		ray.y_step = 0;
	ray.x = (data->x / CASE) * CASE;
	if (ray.x_multi == 1)
		ray.x += CASE;
	ray.y = data->y + (ray.x - data->x) * tan(ray.angle);
	while (TRUE)
	{
		if (ray.y < 0 || ray.y > data->minimap.height * CASE
			|| ray.x < 0 || ray.x > data->minimap.width * CASE
			|| ray.y - ray.y_multi < 0 || ray.x - ray.x_multi < 0
			|| ray.y - ray.y_multi > data->minimap.height * CASE
			|| ray.x - ray.x_multi > data->minimap.width * CASE)
			break ;
		if (data->map[(int)ray.y / CASE][(int)(ray.x) / CASE] == ' '
			|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' '
			|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)ray.x / CASE] == ' '
			|| data->map[(int)ray.y / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' ')
			break ;
		ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);
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

	ray.angle = (data->angle - data->fov_rad / 2) + W;
	ray.map = data->map;
	i_ray = 0;
	while (i_ray < data->minimap.width * CASE)
	{
		ray.angle = fmod(ray.angle, N);
		set_ray_multi(&ray);
		h_ray = get_h_ray(data, ray);
		v_ray = get_v_ray(data, ray);
		ray = v_ray;
		if (h_ray.distance > v_ray.distance)
			ray = h_ray;/* 
		if (!(ray.y <= 0 || ray.y >= data->minimap.height * CASE
			|| ray.x <= 0 || ray.x >= data->minimap.width * CASE))
			ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF); */
		// draw_ray_on_minimap(data, ray);
		// draw_wall(data, ray);
		++i_ray;
		ray.angle += data->fov_rad / (data->minimap.width * CASE);
	}
}
