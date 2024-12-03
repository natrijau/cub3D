#include "cub3d_bonus.h"

void	draw_minimap(t_data *data, double distance, int x, int y)
{
	if (distance <= data->calculs.case_div_two)
		ft_mlx_pixel_put(&data->img_win,
			data->calculs.minimap_pos_x + x, data->calculs.minimap_pos_y + y, 0x00FF0000);
	else if (distance <= data->calculs.h_div_ten)
	{
		if (x + data->x - data->calculs.h_div_ten >= 0
			&& y + data->y - data->calculs.h_div_ten >= 0
			&& x + data->x - data->calculs.h_div_ten < data->width_and_case
			&& y + data->y - data->calculs.h_div_ten < data->height_and_case
			&& data->map[(int)((y + data->y - data->calculs.h_div_ten) / CASE)]
			[(int)((x + data->x - data->calculs.h_div_ten) / CASE)] == '0')
			ft_mlx_pixel_put(&data->img_win, data->calculs.minimap_pos_x + x,
				data->calculs.minimap_pos_y + y, 0x00A0A0A0);
		else
			ft_mlx_pixel_put(&data->img_win, data->calculs.minimap_pos_x + x,
				data->calculs.minimap_pos_y + y, 0x00000000);
	}
	else if (distance <= data->calculs.h_div_ten + 1)
		ft_mlx_pixel_put(&data->img_win, data->calculs.minimap_pos_x + x,
			data->calculs.minimap_pos_y + y, 0x00FFFFFF);
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
void	init_minimap(t_data *data)
{
	double	distance;
	int		x;
	int		y;

	y = 0;
	while (y < data->calculs.h_div_five)
	{
		x = 0;
		while (x < data->calculs.h_div_five)
		{
			distance = sqrt(pow(x - data->calculs.h_div_ten, 2) + pow(y - data->calculs.h_div_ten, 2));
			draw_minimap(data, distance, x, y);
			++x;
		}
		++y;
	}
}
