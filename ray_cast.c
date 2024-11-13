#include "cub3d.h"

/*initializes the radius parameters for each angle.*/
void	ray_setup(t_data *data, t_ray *ray)
{
	ray->angle = fmod(ray->angle, N); // Limiter l'angle à une valeur comprise entre 0 et N
	if (ray->angle > W)               // Si l'angle est supérieur à W
		ray->angle -= N;                                                                                                                                                                                        // Réduire l'angle de N pour le remettre dans la plage
	ray->x = data->x;                 // Position en x du rayon égale à la position actuelle de l'utilisateur
	ray->y = data->y;                 // Position en y du rayon égale à la position actuelle de l'utilisateur
	ray->x_multi = -1;                // Initialisation de x_multi
	if (ray->angle > W && ray->angle < E)  // Si l'angle est entre W et E
		ray->x_multi = 1;             // Inverser x_multi
	ray->y_multi = -1;                 // Initialisation de y_multi
	if (ray->angle < N && ray->angle > S)  // Si l'angle n'est pas entre N et S
		ray->y_multi = 1;        // Inverser y_multi
	ray->x_step = cos(ray->angle) * ray->x_multi;  // Calculer le pas en x selon l'angle
	ray->y_step = sin(ray->angle) * ray->x_multi;  // Calculer le pas en y selon l'angle
	ray->x_step += cos(ray->angle + W) * ray->y_multi;  // Ajouter la composante de rotation W pour x
	ray->y_step += sin(ray->angle + W) * ray->y_multi;  // Ajouter la composante de rotation W pour y
	ray->flag = 'x';
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

/* vérifie que le rayon ne sort pas de la carte ou ne rencontre pas un espace vide ( collision) ? */
void    ray_cast_projection(t_data *data, t_ray *ray)
{

    // Vérifie si le rayon sort de la carte ou rencontre un mur
    if (ray->y >= 0 && ray->y <= data->height_and_case
        && ray->x >= 0 && ray->x <= data->width_and_case
        && data->map[(int)ray->y / CASE][(int)ray->x / CASE] == '0'
        && data->map[(int)(ray->y - ray->y_step) / CASE][(int)ray->x / CASE] == '0'
        && data->map[(int)ray->y / CASE][(int)(ray->x - ray->x_step) / CASE] == '0')
	{
		if (sqrt(pow(ray->x - data->x, 2) + pow(ray->y - data->y, 2)) <= HEIGHT / 10)
		{
			ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + (ray->x - data->x + HEIGHT / 10) - ray->x_step / 2, MINIMAP_IMG_POS_Y + (ray->y - data->y + HEIGHT / 10) - ray->y_step / 2, 0x00FFFFFF); // pour faire un jolie rayon
			ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + (ray->x - data->x + HEIGHT / 10), MINIMAP_IMG_POS_Y + (ray->y - data->y + HEIGHT / 10), 0x00FFFFFF);
		}
		if (ray->flag == 'x')
		{
    		ray->x += ray->x_step;
			ray->flag = 'y';
		}
		else
		{
    		ray->y += ray->y_step;
			ray->flag = 'x';
		}
		ray_cast_projection(data, ray);
        return ;
	}
	if (fabs(ray->x_step) > 0.01 || fabs(ray->y_step) > 0.01)
	{
		
		ray->x_step /= 2;
		ray->y_step /= 2;
    	ray->x -= ray->x_step;
    	ray->y -= ray->y_step;
		ray_cast_projection(data, ray);
        return ;
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
		ray_setup(data, &ray);  // Préparer le rayon
		ray.x += ray.x_step;
		ray.y += ray.y_step;
		ray_cast_projection(data, &ray);
		draw_wall(data, ray, i_ray);  // Dessiner le mur à cette distance
		++i_ray;
		ray.angle += data->angle_step;
	}
}
