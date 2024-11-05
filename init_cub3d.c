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

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));  // Calcul de la position du pixel
	*(unsigned int *)dst = color;  // Placer la couleur au bon endroit
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
	data->map_height = ft_strtablen(data->map) * CASE;  // Définir la hauteur de la minimap
	data->map_width = ft_strlen(data->map[0]) * CASE;  // Définir la largeur de la minimap
	data->raycast = init_ray_cast(data);  // Initialiser l'image du raycasting
	data->hook.keyboard_bool = FALSE;
	data->hook.move_forward = FALSE;
	data->hook.move_back = FALSE;
	data->hook.move_right = FALSE;
	data->hook.move_left = FALSE;
	data->hook.rotate_left = FALSE;
	data->hook.rotate_right = FALSE;
	ray_cast(data);  // Lancer le calcul de raycasting (projection 3D simulée)
	mlx_put_image_to_window(data->mlx, data->win, data->raycast.raycast.img, 0, 0);
	return (0);
}
