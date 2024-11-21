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
	if (data->map[(int)ray.y / CASE][(int)ray.x / CASE] == 'D')
		raycast->actual_wall = raycast->D_wall;
}

/*dessine le mur à la bonne distance en fonction de la projection du rayon ?*/
void	draw_wall(t_data *data, t_ray ray, int x)
{
	t_raycast	raycast;
	double		factor;
	double		y;

	raycast = data->raycast;
	set_texture_config(data, ray, &raycast);
	raycast.x *= (double)raycast.actual_wall.width / CASE;
	raycast.distance *= cos(fmod(ray.angle - (data->angle + (M_PI / 4)), N));
	raycast.distance = (CASE / raycast.distance) * ((WIDTH >> 1) / tan(data->fov_rad / 2));
	factor = (double)raycast.actual_wall.height / raycast.distance;
	y = (HEIGHT >> 1) - raycast.distance / 2;
	if (y < 0)
		y = 0;
	raycast.y = (y - (HEIGHT >> 1) + raycast.distance / 2) * factor;
	if (raycast.y < 0)
		raycast.y = 0;
	double	x_pow;
	x_pow = pow(x - (WIDTH - H_DIV_TEN), 2);
	while (y < (HEIGHT >> 1) + raycast.distance / 2 && y <= HEIGHT)
	{
		raycast.wall_color = ft_mlx_get_pixel_color(&raycast.actual_wall, raycast.x, raycast.y);
		if ((x < MINIMAP_POS_X && y < MINIMAP_POS_Y)
			|| sqrt(x_pow + pow(y - (HEIGHT - H_DIV_TEN), 2)) > H_DIV_TEN + 1)
			ft_mlx_pixel_put(&data->img_win, x, y, raycast.wall_color);
		raycast.y += factor;
		++y;
	}
}

int	change_door_case(t_data *data, int x, int y)
{
	int	i;

	if (x == ((int)data->x / CASE) && y == ((int)data->y / CASE))
		return (0);
	i = 0;
	while(data->tab_door[i])
	{
		if (data->tab_door[i][0] == x && data->tab_door[i][1] == y)
		{
			if (data->map[y][x] == 'D')
				data->map[y][x] = '0';
			else
				data->map[y][x] = 'D';
			data->change_state_door = FALSE;
			return (1);
		}
		++i;
	}
	return (0);
}

/* vérifie que le rayon ne sort pas de la carte ou ne rencontre pas un espace vide ( collision) ? */
void	ray_cast_projection(t_data *data, t_ray *ray, int check_wall)
{
	data->raycast.distance = sqrt(pow(ray->x - data->x, 2) + pow(ray->y - data->y, 2));
	ray->x_fabs_step = fabs(ray->x_step);
	ray->y_fabs_step = fabs(ray->y_step);
	while (data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& (data->map[(int)(ray->y - ray->y_step) / CASE][(int)ray->x / CASE] == '0'
		|| data->map[(int)ray->y / CASE][(int)(ray->x - ray->x_step) / CASE] == '0'))
	{
		if (data->raycast.distance <= H_DIV_TEN)
		{
			if (ray->x_fabs_step > 1 || ray->y_fabs_step > 1)
				ft_mlx_pixel_put(&data->img_win, RAY_POS_X + (ray->x - data->x) - ray->x_step / 2, RAY_POS_Y + (ray->y - data->y) - ray->y_step / 2, 0x00FFFFFF);
			ft_mlx_pixel_put(&data->img_win, RAY_POS_X + (ray->x - data->x), RAY_POS_Y + (ray->y - data->y), 0x00FFFFFF); 
		}
		ray->x += ray->x_step;
		ray->y += ray->y_step;
		data->raycast.distance = sqrt(pow(ray->x - data->x, 2) + pow(ray->y - data->y, 2));
		if (check_wall && data->raycast.distance <= CASE
			&& change_door_case(data, (int)ray->x / CASE, (int)ray->y / CASE))
			check_wall = FALSE;
	}
	if (ray->x_fabs_step > 0.1 || ray->y_fabs_step > 0.1)
	{
		ray->x -= ray->x_step;
		ray->y -= ray->y_step;
		ray->x_step *= 0.1;
		ray->y_step *= 0.1;
		ray_cast_projection(data, ray, check_wall);
	}
}

/*c'est la boucle principale qui parcourt l'écran en projetant des rayons, calcul 
les collisions, et dessine les murs ?*/
void	ray_cast(t_data *data)
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
			ray_cast_projection(data, &ray, TRUE);
		else
			ray_cast_projection(data, &ray, FALSE);
		draw_wall(data, ray, i_ray);
		++i_ray;
		ray.angle += data->angle_step;
	}
}
