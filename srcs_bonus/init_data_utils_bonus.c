/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:49:56 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 12:49:56 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	init_calculs(t_data *data)
{
	data->calculs.rotate_speed = (2 * M_PI / 90);
	data->calculs.case_div_two = CASE / 2;
	data->calculs.h_div_ten = HEIGHT / 10;
	data->calculs.h_div_five = HEIGHT / 5;
	data->calculs.minimap_pos_x = WIDTH - data->calculs.h_div_five - 1;
	data->calculs.minimap_pos_y = HEIGHT - data->calculs.h_div_five - 1;
	data->calculs.ray_pos_x = data->calculs.minimap_pos_x
		+ data->calculs.h_div_ten;
	data->calculs.ray_pos_y = data->calculs.minimap_pos_y
		+ data->calculs.h_div_ten;
	data->calculs.north = M_PI * 2;
	data->calculs.east = M_PI * 1.5;
	data->calculs.south = M_PI * 1;
	data->calculs.west = M_PI * 0.5;
}

int	check_color_value(char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j] && i < 3)
		{
			if (!ft_isdigit(tab[i][j]) || j > 2)
				return (-1);
			j++;
		}
		if (ft_atoi(tab[i]) > 255 || i > 2)
			return (-1);
		i++;
	}
	return (0);
}

int	get_wall(t_image *dest, t_data *data, char *str, char *direction)
{
	if (!ft_strncmp(str, direction, 2))
	{
		if (dest->img)
		{
			printf("Error\nDouble texture definition: %s\n", direction);
			return (-1);
		}
		dest->img = mlx_xpm_file_to_image(data->mlx, &str[2],
				&dest->width, &dest->height);
		if (!dest->img)
		{
			printf("Error\nFailed texture importation: %s\n", &str[2]);
			return (-1);
		}
		dest->addr = mlx_get_data_addr(dest->img, &dest->bpp,
				&dest->line_len, &dest->endian);
	}
	return (0);
}

// returns the starting angle based on the direction
double	get_angle(t_data *data, char direction)
{
	if (direction == 'N')
		return (data->calculs.north);
	if (direction == 'E')
		return (data->calculs.west);
	if (direction == 'S')
		return (data->calculs.south);
	if (direction == 'W')
		return (data->calculs.east);
	printf("Error\nInvalid player direction: %c\n", direction);
	return (-1);
}
