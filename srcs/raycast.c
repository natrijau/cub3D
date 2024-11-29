#include "cub3d.h"

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
			raycast->actual_wall = raycast->N_wall;
			raycast->x = CASE - raycast->x;
		}
		else
			raycast->actual_wall = raycast->S_wall;
	}
	else
	{
		raycast->x = fmod(ray.y, CASE);
		if (ray.x > data->x)
			raycast->actual_wall = raycast->W_wall;
		else
		{
			raycast->x = CASE - raycast->x;
			raycast->actual_wall = raycast->E_wall;
		}
	}
	raycast->x *= (double)raycast->actual_wall.width / CASE;
}

/*dessine le mur à la bonne distance en fonction de la projection du rayon ?*/
void	draw_wall(t_data *data, int x)
{
	t_raycast	raycast;
	double		factor;
	double		y;

	raycast = data->raycast;
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

/* vérifie que le rayon ne sort pas de la carte ou 
ne rencontre pas un espace vide ( collision) ? */
void	raycast_projection(t_data *data, t_ray *ray)
{
	while (data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& (data->map[(int)(ray->y - ray->y_step) / CASE]
		[(int)ray->x / CASE] == '0' || data->map[(int)ray->y / CASE]
		[(int)(ray->x - ray->x_step) / CASE] == '0'))
	{
		ray->x += ray->x_step;
		ray->y += ray->y_step;
	}
	if (fabs(ray->x_step) > 0.1 || fabs(ray->y_step) > 0.1)
	{
		ray->x -= ray->x_step;
		ray->y -= ray->y_step;
		ray->x_step *= 0.1;
		ray->y_step *= 0.1;
		raycast_projection(data, ray);
	}
}

/*c'est la boucle principale qui parcourt l'écran en projetant des rayons, 
calcul les collisions, et dessine les murs ?*/
void	raycast(t_data *data)
{
	t_ray	ray;
	int		i_ray;

	ray.angle = data->angle - data->first_rayangle;
	i_ray = 0;
	while (i_ray < WIDTH)
	{
		ray.angle = fmod(ray.angle, N);
		ray.x_step = cos(ray.angle + M_PI) + cos(ray.angle + E);
		ray.y_step = sin(ray.angle + M_PI) + sin(ray.angle + E);
		ray.x = data->x + ray.x_step;
		ray.y = data->y + ray.y_step;
		raycast_projection(data, &ray);
		data->raycast.distance = sqrt(pow(data->x - ray.x, 2)
				+ pow(data->y - ray.y, 2)) * cos(fmod(ray.angle
					- (data->angle + (M_PI / 4)), N));
		set_texture_config(data, ray, &data->raycast);
		draw_wall(data, i_ray);
		++i_ray;
		ray.angle += data->angle_step;
	}
}
