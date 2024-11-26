#include "cub3d.h"

void	draw_minimap(t_data *data, double distance, int x, int y)
{
	if (distance <= CASE_DIV_TWO)
		ft_mlx_pixel_put(&data->img_win,
			MINIMAP_POS_X + x, MINIMAP_POS_Y + y, 0x00FF0000);
	else if (distance <= H_DIV_TEN)
	{
		if (x + data->x - H_DIV_TEN >= 0
			&& y + data->y - H_DIV_TEN >= 0
			&& x + data->x - H_DIV_TEN < data->width_and_case
			&& y + data->y - H_DIV_TEN < data->height_and_case
			&& data->map[(int)((y + data->y - H_DIV_TEN) / CASE)]
			[(int)((x + data->x - H_DIV_TEN) / CASE)] == '0')
			ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
				MINIMAP_POS_Y + y, 0x00A0A0A0);
		else
			ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
				MINIMAP_POS_Y + y, 0x00000000);
	}
	else if (distance <= H_DIV_TEN + 1)
		ft_mlx_pixel_put(&data->img_win, MINIMAP_POS_X + x,
			MINIMAP_POS_Y + y, 0x00FFFFFF);
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
void	init_minimap(t_data *data)
{
	double	distance;
	int		x;
	int		y;

	y = 0;
	while (y < H_DIV_FIVE)
	{
		x = 0;
		while (x < H_DIV_FIVE)
		{
			distance = sqrt(pow(x - H_DIV_TEN, 2) + pow(y - H_DIV_TEN, 2));
			draw_minimap(data, distance, x, y);
			++x;
		}
		++y;
	}
}
