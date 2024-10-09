#include "cub3d.h"

void	ray_setup(t_data *data, t_ray *ray)
{
	ray->angle = fmod(ray->angle, N);
	if (ray->angle > W)
		ray->angle -= N;
	ray->x = data->x;
	ray->y = data->y;
	ray->x_multi = -1;
	if (ray->angle > W && ray->angle < E)
		ray->x_multi = 1;
	ray->y_multi = 1;
	if (!(ray->angle < N && ray->angle > S))
		ray->y_multi = -1;
	ray->x_step = cos(ray->angle) * ray->x_multi;
	ray->y_step = sin(ray->angle) * ray->x_multi;
	ray->x_step += cos(ray->angle + W) * ray->y_multi;
	ray->y_step += sin(ray->angle + W) * ray->y_multi;
}

int	ray_cast_protection(t_data *data, t_ray ray)
{
	if (ray.y < 0 || ray.y > data->minimap.height * CASE
		|| ray.x < 0 || ray.x > data->minimap.width * CASE)
		return (-1);
	if (data->map[(int)ray.y / CASE][(int)(ray.x) / CASE] == ' ')
		return (-1);
	return (0);
}

void	draw_wall(t_data *data, t_ray ray, int x)
{
	int		y;
	double	distance;

	ray.distance = sqrt(pow(ray.x - data->x, 2) + pow(ray.y - data->y, 2));
	ray.distance *= cos(fmod(ray.angle - (data->angle + M_PI / 4), N));
	distance = (CASE / ray.distance) * ((WIDTH / 2) / tan(data->fov_rad / 2));
	y = (HEIGHT / 2) - distance / 2;
	if (y < 0)
		y = 0;
	while (y < (HEIGHT / 2) + distance / 2 && y <= HEIGHT)
	{
			ft_mlx_pixel_put(&data->minimap.raycast, x, y, 0x00FFFFFF);
		++y;
	}
}

void	ray_cast(t_data *data)
{
	t_ray	ray;
	int		i_ray;

	ray.angle = data->angle - (data->fov_rad / 2.0) + M_PI / 4;
	i_ray = 0;
	while (i_ray < WIDTH)
	{
		ray_setup(data, &ray);
		while (TRUE)
		{
			ray.x += ray.x_step / 10;
			ray.y += ray.y_step / 10;
			if (ray_cast_protection(data, ray) == -1)
				break ;
			ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);
		}
		draw_wall(data, ray, i_ray);
		++i_ray;
		ray.angle += data->fov_rad / WIDTH;
	}
}
