#include "cub3d.h"

/*dessine le mur à la bonne distance en fonction de la projection du rayon ?*/
void	draw_wall(t_data *data, int x)
{
	t_raycast	raycast;
	double		factor;
	double		y;
	double		x_pow;

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
	x_pow = pow(x - (WIDTH - H_DIV_TEN), 2);
	while (++y < (HEIGHT >> 1) + raycast.distance / 2 && y <= HEIGHT)
	{
		raycast.wall_color = ft_mlx_get_pixel_color(&raycast.actual_wall,
				raycast.x, raycast.y);
		if ((x < MINIMAP_POS_X && y < MINIMAP_POS_Y)
			|| sqrt(x_pow + pow(y - (HEIGHT - H_DIV_TEN), 2)) > H_DIV_TEN + 1)
			ft_mlx_pixel_put(&data->img_win, x, y, raycast.wall_color);
		raycast.y += factor;
	}
}

void	steps_progression(t_data *data, t_ray *ray, int *check_wall)
{
	while (data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& (data->map[(int)(ray->y - ray->y_step) / CASE]
		[(int)ray->x / CASE] == '0' || data->map[(int)ray->y / CASE]
		[(int)(ray->x - ray->x_step) / CASE] == '0'))
	{
		if (data->raycast.distance <= H_DIV_TEN)
		{
			if (ray->x_fabs_step > 1 || ray->y_fabs_step > 1)
				ft_mlx_pixel_put(&data->img_win,
					RAY_POS_X + (ray->x - data->x) - ray->x_step / 2,
					RAY_POS_Y + (ray->y - data->y) - ray->y_step / 2,
					0x00FFFFFF);
			ft_mlx_pixel_put(&data->img_win, RAY_POS_X + (ray->x - data->x),
				RAY_POS_Y + (ray->y - data->y), 0x00FFFFFF);
		}
		ray->x += ray->x_step;
		ray->y += ray->y_step;
		data->raycast.distance = sqrt(pow(ray->x - data->x, 2)
				+ pow(ray->y - data->y, 2));
		if (*check_wall && data->raycast.distance <= CASE
			&& change_door_case(data, (int)ray->x / CASE, (int)ray->y / CASE))
			*check_wall = FALSE;
	}
}

/* vérifie que le rayon ne sort pas de la carte ou 
ne rencontre pas un espace vide ( collision) ? */
void	raycast_projection(t_data *data, t_ray *ray, int check_wall)
{
	data->raycast.distance = sqrt(pow(ray->x - data->x, 2)
			+ pow(ray->y - data->y, 2));
	ray->x_fabs_step = fabs(ray->x_step);
	ray->y_fabs_step = fabs(ray->y_step);
	steps_progression(data, ray, &check_wall);
	if (ray->x_fabs_step > 0.1 || ray->y_fabs_step > 0.1)
	{
		ray->x -= ray->x_step;
		ray->y -= ray->y_step;
		ray->x_step *= 0.1;
		ray->y_step *= 0.1;
		raycast_projection(data, ray, check_wall);
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
		if (i_ray == WIDTH >> 1 && data->change_state_door)
			raycast_projection(data, &ray, TRUE);
		else
			raycast_projection(data, &ray, FALSE);
		data->raycast.distance *= cos(fmod(ray.angle
					- (data->angle + (M_PI / 4)), N));
		set_texture_config(data, ray, &data->raycast);
		draw_wall(data, i_ray);
		++i_ray;
		ray.angle += data->angle_step;
	}
}
