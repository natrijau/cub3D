#include "cub3d.h"

void	draw_line(t_image *img, t_vec start, t_vec end)
{
	int m_new = 2 * (end.y - start.y);
    int slope_error_new = m_new - (end.x - start.x); 
    for (int x = start.x, y = start.y; x <= end.x; x++) { 
        ft_mlx_pixel_put(img, x, y, 0x00FFFFFF);
        slope_error_new += m_new;
        if (slope_error_new >= 0) { 
            y++; 
            slope_error_new -= 2 * (end.x - start.x);
        }
    }
}

/*c'est la boucle principale qui parcourt l'écran en projetant des rayons, calcul 
les collisions, et dessine les murs ?*/
void	ray_cast(t_data *data)
{
	t_ray	ray;
	int		i_ray;

	ray.angle = data->angle - data->first_rayangle;  // Départ du rayon à l'angle de vision
	i_ray = 0;
	while (i_ray < WIDTH)  // Parcourir la largeur de l'écran
	{
		ray_setup(data, &ray);  // Préparer le rayon
		while (TRUE)  // Lancer le rayon jusqu'à ce qu'il rencontre un obstacle
		{
			ray.x += ray.x_step * 0.1;
			if (ray_cast_protection(data, ray) == -1)  // Vérifier les collisions
				break;
			ray.flag = 'x';
			ray.y += ray.y_step * 0.1;
			if (ray_cast_protection(data, ray) == -1)  // Vérifier les collisions
				break;
			ray.flag = 'y';
			ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x00FFFFFF);  // Dessiner le rayon (en bleu ici)
		}
		// draw_line(&data->minimap.space, (t_vec){data->x, data->y}, (t_vec){ray.x, ray.y});
		draw_wall(data, ray, i_ray);  // Dessiner le mur à cette distance
		++i_ray;
		ray.angle += data->angle_step;  // Incrémenter l'angle du rayon pour le prochain
	}
}