#include "cub3d.h"

// Fonction pour placer un pixel de couleur spécifique dans une image
void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));  // Calcul de la position du pixel
	*(unsigned int *)dst = color;  // Placer la couleur au bon endroit
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
t_image	init_space(t_data *data)
{
	t_image	img;
	int		color;
	int		x, y, i, j;

	img.img = mlx_new_image(data->mlx, data->minimap.width * CASE, data->minimap.height * CASE);  // Créer une nouvelle image de la taille de la minimap
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);  // Obtenir l'adresse de l'image
	color = 0x00FFFFFF;  // Blanc pour représenter l'espace vide
	y = -1;
	while (++y < data->minimap.height)  // Parcourir les lignes de la minimap
	{
		x = -1;
		while (++x < data->minimap.width && data->map[y][x])  // Parcourir les colonnes de la minimap
		{
			if (data->map[y][x] == '0')  // Si la case est un espace vide ('0')
			{
				i = -1;
				while (++i < CASE)  // Parcourir les pixels de la case
				{
					j = -1;
					while (++j < CASE)  // Remplir la case avec des pixels blancs
						ft_mlx_pixel_put(&img, x * CASE + i, y * CASE + j, color);
				}
			}
		}
	}
	return (img);  // Retourner l'image de l'espace
}

// Fonction pour initialiser l'image représentant le personnage sur la minimap
t_image	init_character(void *mlx)
{
	t_image	img;
	int		color;
	int		i;
	int		j;

	img.img = mlx_new_image(mlx, CASE, CASE);  // new image for character
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	color = 0x00FF0000;  // red
	i = 0;
	while (i < CASE)
	{
		j = 0;
		while (j < CASE) // Fill the image with red pixels
		{
			ft_mlx_pixel_put(&img, i, j, color);
			++j;
		}
		++i;
	}
	return (img);
}

// Fonction pour initialiser l'image du raycasting (vue en 3D simulée)
t_image	init_ray_cast(void *mlx)
{
	t_image	img;

	img.img = mlx_new_image(mlx, WIDTH, HEIGHT);  // Créer une nouvelle image pour le raycasting
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	return (img);  // Retourner l'image du raycasting
}

// Fonction pour créer et afficher la minimap et ses composants
int	create_minimap(t_data *data)
{
	data->minimap.height = ft_strtablen(data->map);  // Définir la hauteur de la minimap
	data->minimap.width = ft_strlen(data->map[0]);  // Définir la largeur de la minimap
	data->minimap.space = init_space(data);  // Initialiser l'image de l'espace (fond de la minimap)
	data->minimap.character = init_character(data->mlx);  // Initialiser l'image du personnage
	data->minimap.raycast = init_ray_cast(data->mlx);  // Initialiser l'image du raycasting
	ray_cast(data);  // Lancer le calcul de raycasting (projection 3D simulée)
	
	// Afficher les images à la fenêtre (rendu final sur l'écran)
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - CASE / 2, data->y - CASE / 2);

	return (0);
}
