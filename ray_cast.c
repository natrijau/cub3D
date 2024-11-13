#include "cub3d.h"

/*initializes the radius parameters for each angle.*/
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
	ray->y_multi = -1;
	if (ray->angle < N && ray->angle > S)
		ray->y_multi = 1;
	ray->x_step = cos(ray->angle) * ray->x_multi;
	ray->y_step = sin(ray->angle) * ray->x_multi;
	ray->x_step += cos(ray->angle + W) * ray->y_multi;
	ray->y_step += sin(ray->angle + W) * ray->y_multi;
	ray->x_step_div = ray->x_step * 0.1;
	ray->y_step_div = ray->y_step * 0.1;
}

/* vérifie que le rayon ne sort pas de la carte ou ne 
	rencontre pas un espace vide ( collision) ? */
int	ray_cast_protection(t_data *data, t_ray *ray)
{
	int	i;

	if (ray->y >= 0 && ray->y <= data->height_and_case
		&& ray->x >= 0 && ray->x <= data->width_and_case
		&& data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& data->map[(int)(ray->y - ray->y_step) / CASE][(int)ray->x / CASE] == '0'
		&& data->map[(int)ray->y / CASE][(int)(ray->x - ray->x_step) / CASE] == '0')
		return (0);
	ray->x -= ray->x_step;
	ray->y -= ray->y_step;
	i = 0;
	while (ray->y >= 0 && ray->y <= data->height_and_case
		&& ray->x >= 0 && ray->x <= data->width_and_case
		&& data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
		&& data->map[(int)(ray->y - ray->y_step_div) / CASE][(int)ray->x / CASE] == '0'
		&& data->map[(int)ray->y / CASE][(int)(ray->x - ray->x_step_div) / CASE] == '0')
	{
		if (i++ % 2)
		{
			ray->x += ray->x_step_div;
			ray->flag = 'y';
		}
		else
		{
			ray->y += ray->y_step_div;
			ray->flag = 'x';
		}
	}
	return (-1);
}

int	ft_mlx_get_pixel_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp >> 3));
	return (*(unsigned int *)dst);
}

void	set_texture_config(t_data *data, t_ray ray, t_raycast *raycast)
{
	if (data->map[(int)ray.y / CASE][(int)ray.x / CASE] == 'D')
	{
		raycast->actual_wall = raycast->D_wall;
		raycast->x = fmod(ray.x, CASE);
	}
	else if (ray.flag == 'x')
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
	raycast.distance = sqrt(pow(ray.x - data->x, 2) + pow(ray.y - data->y, 2));
	raycast.distance *= cos(fmod(ray.angle - (data->angle + M_PI / 4), N));
	raycast.distance = (CASE / raycast.distance) * ((WIDTH >> 1) / tan(data->fov_rad / 2));
	factor = (double)raycast.actual_wall.height / raycast.distance;
	y = (HEIGHT >> 1) - raycast.distance / 2;
	if (y < 0)
		y = 0;
	raycast.y = (y - (HEIGHT >> 1) + raycast.distance / 2) * factor;
	if (raycast.y < 0)
		raycast.y = 0;
	while (y < (HEIGHT >> 1) + raycast.distance / 2 && y <= HEIGHT)
	{
		raycast.wall_color = ft_mlx_get_pixel_color(&raycast.actual_wall, raycast.x, raycast.y);
		if ((x < WIDTH - HEIGHT / 5 && y < HEIGHT - HEIGHT / 5)
			|| sqrt(pow(x - (WIDTH - HEIGHT / 10), 2) + pow(y - (HEIGHT - HEIGHT / 10), 2)) > HEIGHT / 10 + 1)
			ft_mlx_pixel_put(&data->img_win, x, y, raycast.wall_color);
		raycast.y += factor;
		++y;
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
		ray_setup(data, &ray);
		while (TRUE)
		{
			ray.x += ray.x_step;
			ray.y += ray.y_step;
			if (ray_cast_protection(data, &ray) == -1)
				break ;
			if (sqrt(pow(ray.x - data->x, 2) + pow(ray.y - data->y, 2)) <= HEIGHT / 10)
			{
				ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + (ray.x - data->x + HEIGHT / 10) - ray.x_step / 2, MINIMAP_IMG_POS_Y + (ray.y - data->y + HEIGHT / 10) - ray.y_step / 2, 0x00FFFFFF);
				ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + (ray.x - data->x + HEIGHT / 10), MINIMAP_IMG_POS_Y + (ray.y - data->y + HEIGHT / 10), 0x00FFFFFF);
			}
		}
		draw_wall(data, ray, i_ray);
		++i_ray;
		ray.angle += data->angle_step;
	}
}
