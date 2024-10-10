#include "cub3d.h"
	
/*initialise les paramètres du rayon pour chaque angle.*/
void	ray_setup(t_data *data, t_ray *ray)
{
	ray->angle = fmod(ray->angle, N); // Limiter l'angle à une valeur comprise entre 0 et N
	if (ray->angle > W)               // Si l'angle est supérieur à W
		ray->angle -= N;              // Réduire l'angle de N pour le remettre dans la plage
	ray->x = data->x;                 // Position en x du rayon égale à la position actuelle de l'utilisateur
	ray->y = data->y;                 // Position en y du rayon égale à la position actuelle de l'utilisateur
	ray->x_multi = -1;                // Initialisation de x_multi
	if (ray->angle > W && ray->angle < E)  // Si l'angle est entre W et E
		ray->x_multi = 1;             // Inverser x_multi
	ray->y_multi = 1;                 // Initialisation de y_multi
	if (!(ray->angle < N && ray->angle > S))  // Si l'angle n'est pas entre N et S
		ray->y_multi = -1;            // Inverser y_multi
	ray->x_step = cos(ray->angle) * ray->x_multi;  // Calculer le pas en x selon l'angle
	ray->y_step = sin(ray->angle) * ray->x_multi;  // Calculer le pas en y selon l'angle
	ray->x_step += cos(ray->angle + W) * ray->y_multi;  // Ajouter la composante de rotation W pour x
	ray->y_step += sin(ray->angle + W) * ray->y_multi;  // Ajouter la composante de rotation W pour y
}

/*
vérifie que le rayon ne sort pas de la carte ou ne rencontre pas un espace vide (ce qui indiquerait une collision).
*/
int	ray_cast_protection(t_data *data, t_ray ray)
{
	// Vérifie si le rayon sort de la carte ou rencontre un mur
	if (ray.y < 0 || ray.y > data->minimap.height * CASE
		|| ray.x < 0 || ray.x > data->minimap.width * CASE)
		return (-1);
	if (data->map[(int)ray.y / CASE][(int)(ray.x) / CASE] == ' '
		|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' '
		|| data->map[(int)(ray.y - ray.y_multi) / CASE][(int)ray.x / CASE] == ' '
		|| data->map[(int)ray.y / CASE][(int)(ray.x - ray.x_multi) / CASE] == ' ')
		return (-1);
	return (0);
}

/*dessine le mur à la bonne distance en fonction de la projection du rayon.*/
void	draw_wall(t_data *data, t_ray ray, int x)
{
	int		y;
	double	distance;

	// Ajuster la distance du rayon en fonction de l'angle pour corriger la distorsion
	ray.distance *= cos(fmod(ray.angle - (data->angle + M_PI / 4), N));
	distance = (CASE / ray.distance) * ((WIDTH / 2) / tan(data->fov_rad / 2));  // Calcul de la distance corrigée pour le rendu
	y = (HEIGHT / 2) - distance / 2;  // Position de départ du dessin du mur
	while (y < (HEIGHT / 2) + distance / 2)
	{
		if (y >= 0 && y <= HEIGHT)  // S'assurer que l'on dessine dans les limites de l'écran
			ft_mlx_pixel_put(&data->minimap.raycast, x, y, 0x00FFFFFF);  // Dessiner un pixel blanc pour représenter le mur
		++y;
	}
}

/*c'est la boucle principale qui parcourt l'écran en projetant des rayons, calculant 
les collisions, et dessinant les murs.*/
void	ray_cast(t_data *data)
{
	t_ray	ray;
	int		i_ray;

	ray.angle = data->angle - (data->fov_rad / 2.0) + M_PI / 4;  // Départ du rayon à l'angle de vision
	i_ray = 0;
	while (i_ray < WIDTH)  // Parcourir la largeur de l'écran
	{
		ray_setup(data, &ray);  // Préparer le rayon
		while (TRUE)  // Lancer le rayon jusqu'à ce qu'il rencontre un obstacle
		{
			ray.x += ray.x_step;
			ray.y += ray.y_step;
			if (ray_cast_protection(data, ray) == -1)  // Vérifier les collisions
				break;
			ft_mlx_pixel_put(&data->minimap.space, ray.x, ray.y, 0x000000FF);  // Dessiner le rayon (en bleu ici)
		}
		ray.distance = sqrt(pow(ray.x - data->x, 2) + pow(ray.y - data->y, 2));  // Calculer la distance parcourue par le rayon
		draw_wall(data, ray, i_ray);  // Dessiner le mur à cette distance
		++i_ray;
		ray.angle += data->fov_rad / WIDTH;  // Incrémenter l'angle du rayon pour le prochain
	}
}
