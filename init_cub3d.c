#include "cub3d.h"

void	creat_image(t_image *img, void *mlx, int width, int height)
{
	img->img = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img, &img->bpp, &img->line_len, &img->endian);
}

// Fonction pour placer un pixel de couleur spécifique dans une image
void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp >> 3));  // Calcul de la position du pixel
	*(unsigned int *)dst = color;  // Placer la couleur au bon endroit
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
t_image	init_space(t_data *data)
{
	t_image	img;
	int		x, y, i, j;

	creat_image(&img, data->mlx, data->minimap.width * CASE, data->minimap.height * CASE);  // Créer une nouvelle image pour l'espace
	y = -1;
	while (++y < data->minimap.height)  // Parcourir les lignes de la minimap
	{
		x = -1;
		while (++x < data->minimap.width && data->map[y][x])  // Parcourir les colonnes de la minimap
		{
			if ((data->map[y][x] == '0' || data->map[y][x] == 'P') && ((x >= data->x - (CASE >> 1) - 4) || (x <= data->x + 4)))  // Si la case est un espace vide ('0')
			{
				i = -1;
				while (++i < CASE)  // Parcourir les pixels de la case
				{
					j = -1;
					while (++j < CASE)  // Remplir la case avec des pixels blancs
						ft_mlx_pixel_put(&img, x * CASE + i, y * CASE + j, 0x00A0A0A0);
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

	creat_image(&img, mlx, CASE, CASE);  // Créer une nouvelle image pour le personnage
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
t_raycast	init_ray_cast(t_data *data)
{
	t_raycast	raycast;
	int			i;
	int			k;

	raycast = data->raycast;
	creat_image(&raycast.raycast, data->mlx, WIDTH, HEIGHT);  // Créer une nouvelle image pour le raycasting
	k = 0;
	while (k < HEIGHT)
	{
		i = 0;
		while (i < WIDTH)
		{
			if (k < HEIGHT / 2)
				ft_mlx_pixel_put(&raycast.raycast, i, k, raycast.ceiling_color);
			else
				ft_mlx_pixel_put(&raycast.raycast, i, k, raycast.floor_color);
			++i;
		}
		++k;
	}
	return (raycast);
}

// Fonction pour créer et afficher la minimap et ses composants
int	init_cub3d(t_data *data)
{
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "Cub3D");  // New windows
	if (!data->win)
		return (-1);
	data->minimap.height = ft_strtablen(data->map);  // Définir la hauteur de la minimap
	data->minimap.width = ft_strlen(data->map[0]);  // Définir la largeur de la minimap
	data->minimap.space = init_space(data);  // Initialiser l'image de l'espace (fond de la minimap)
	data->minimap.character = init_character(data->mlx);  // Initialiser l'image du personnage
	data->raycast = init_ray_cast(data);  // Initialiser l'image du raycasting
	data->height_and_case = data->minimap.height * CASE;
	data->width_and_case = data->minimap.width * CASE;
	data->hook.keyboard_bool = FALSE;
	data->hook.move_forward = FALSE;
	data->hook.move_back = FALSE;
	data->hook.move_right = FALSE;
	data->hook.move_left = FALSE;
	data->hook.rotate_left = FALSE;
	data->hook.rotate_right = FALSE;
	data->hook.mouse_move = TRUE;
	data->hook.old_x = WIDTH / 2;
	mlx_mouse_move(data->mlx, data->win, data->hook.old_x, HEIGHT / 2);
	ray_cast(data);  // Lancer le calcul de raycasting (projection 3D simulée)
	mlx_put_image_to_window(data->mlx, data->win, data->raycast.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - (CASE >> 1), data->y - (CASE >> 1));
	return (0);
}
