/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:52:05 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/04 13:10:08 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	draw_minimap(t_data *data, double distance, int x, int y)
{
	int	x_case;
	int	y_case;

	if (distance <= data->calculs.case_div_two)
		ft_mlx_pixel_put(&data->img_win,
			data->calculs.minimap_pos_x + x,
			data->calculs.minimap_pos_y + y, 0x00FF0000);
	else if (distance <= data->calculs.h_div_ten)
	{
		x_case = (int)((x + data->x - data->calculs.h_div_ten) / CASE);
		y_case = (int)((y + data->y - data->calculs.h_div_ten) / CASE);
		if (x_case >= 0 && y_case >= 0 && y_case < data->minimap.height
			&& x_case < ft_strlen(data->map[y_case])
			&& data->map[y_case][x_case] == '0')
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
			distance = sqrt(pow(x - data->calculs.h_div_ten, 2)
					+ pow(y - data->calculs.h_div_ten, 2));
			draw_minimap(data, distance, x, y);
			++x;
		}
		++y;
	}
}
