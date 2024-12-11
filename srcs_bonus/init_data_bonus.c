/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:49:47 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 12:49:47 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	init_color(t_data *data, char *str)
{
	static int	count = 0;
	int			color;
	char		**tab;

	color = 0;
	if (str[0] != 'F' && str[0] != 'C')
		return (0);
	tab = ft_split(&str[1], ',');
	if (check_color_value(tab) || ft_strtablen(tab) != 3)
	{
		map_clear(tab);
		printf("Error\nInvalid color value\n");
		return (-1);
	}
	color = (ft_atoi(tab[0]) << 16) | (ft_atoi(tab[1]) << 8) | ft_atoi(tab[2]);
	if (str[0] == 'F')
		data->raycast.floor_color = color;
	else if (str[0] == 'C')
		data->raycast.ceiling_color = color;
	map_clear(tab);
	if (++count > 2 && printf("Error\nToo many color definition\n"))
		return (-1);
	return (0);
}

void	init_fire_texture(t_data *data)
{
	//data->raycast.fire1 = NULL;
	//data->raycast.fire2 = NULL;
	//data->raycast.fire3 = NULL;
	//data->raycast.fire4 = NULL;
	//data->raycast.fire5 = NULL;
	
	data->raycast.fire[0].img = mlx_xpm_file_to_image(data->mlx, "textures/Fire_1-1.xpm", &data->raycast.fire[0].width, &data->raycast.fire[0].height);
	data->raycast.fire[0].addr = mlx_get_data_addr(data->raycast.fire[0].img, &data->raycast.fire[0].bpp, &data->raycast.fire[0].line_len, &data->raycast.fire[0].endian);
	
	data->raycast.fire[1].img = mlx_xpm_file_to_image(data->mlx, "textures/Fire_1-2.xpm", &data->raycast.fire[1].width, &data->raycast.fire[1].height);
	data->raycast.fire[1].addr = mlx_get_data_addr(data->raycast.fire[1].img, &data->raycast.fire[1].bpp, &data->raycast.fire[1].line_len, &data->raycast.fire[1].endian);
	
	data->raycast.fire[2].img = mlx_xpm_file_to_image(data->mlx, "textures/Fire_1-3.xpm", &data->raycast.fire[2].width, &data->raycast.fire[2].height);
	data->raycast.fire[2].addr = mlx_get_data_addr(data->raycast.fire[2].img, &data->raycast.fire[2].bpp, &data->raycast.fire[2].line_len, &data->raycast.fire[2].endian);
	
	data->raycast.fire[3].img = mlx_xpm_file_to_image(data->mlx, "textures/Fire_1-4.xpm", &data->raycast.fire[3].width, &data->raycast.fire[3].height);
	data->raycast.fire[3].addr = mlx_get_data_addr(data->raycast.fire[3].img, &data->raycast.fire[3].bpp, &data->raycast.fire[3].line_len, &data->raycast.fire[3].endian);
	
	data->raycast.fire[4].img = mlx_xpm_file_to_image(data->mlx, "textures/Fire_1-5.xpm", &data->raycast.fire[4].width, &data->raycast.fire[4].height);
	data->raycast.fire[4].addr = mlx_get_data_addr(data->raycast.fire[4].img, &data->raycast.fire[4].bpp, &data->raycast.fire[4].line_len, &data->raycast.fire[4].endian);

}

int	init_texture(t_data *data, char *str)
{
	if (str[0] == 'F' || str[0] == 'C')
		return (0);
	if (ft_strncmp(str, "NO", 2) && ft_strncmp(str, "SO", 2)
		&& ft_strncmp(str, "WE", 2) && ft_strncmp(str, "EA", 2)
		&& ft_strncmp(str, "DO", 2))
	{
		printf("Error\nBad argument: \"%c%c\"\n", str[0], str[1]);
		return (-1);
	}
	if (get_wall(&data->raycast.n_wall, data, str, "NO") == -1)
		return (-1);
	if (get_wall(&data->raycast.s_wall, data, str, "SO") == -1)
		return (-1);
	if (get_wall(&data->raycast.w_wall, data, str, "WE") == -1)
		return (-1);
	if (get_wall(&data->raycast.e_wall, data, str, "EA") == -1)
		return (-1);
	if (get_wall(&data->raycast.door, data, str, "DO") == -1)
		return (-1);
	// test
	init_fire_texture(data);
	return (0);
}

int	find_plyr_pos(t_data *data, char *line, int y, int *find_plyr)
{
	int	x;

	x = 0;
	while (line[x])
	{
		if (ft_strchr("NSEW", line[x]))
		{
			if (*find_plyr)
			{
				printf("Error\nMultiple player positions\n");
				return (-1);
			}
			data->x = ((double)x + 0.5) * CASE;
			data->y = ((double)y + 0.5) * CASE;
			data->angle = get_angle(data, line[x]);
			data->old_angle = data->angle;
			data->fov_rad = (FOV * M_PI) / 180;
			data->first_rayangle = (data->fov_rad / 2.0) - M_PI / 4.0;
			data->angle_step = data->fov_rad / WIDTH;
			line[x] = '0';
			*find_plyr = TRUE;
		}
		x++;
	}
	return (0);
}

int	error_map_start(int start, char **tab)
{
	if (start == 0)
	{
		printf("Error\nInvalid file\n");
		return (-1);
	}
	else if ((!tab[start + 1]))
	{
		printf("Error\nMap not found\n");
		return (-1);
	}
	return (0);
}

int	init_data(t_data *data, char **tab, int map_start)
{
	int	i;
	int	plyr_bool;

	plyr_bool = FALSE;
	i = -1;
	while (tab[++i])
	{
		if (i <= map_start)
		{
			tab[i] = clear_space(tab[i]);
			if (init_color(data, tab[i])
				|| init_texture(data, tab[i]))
				return (-1);
		}
		else if (is_map_line(tab[i], "01NSEWD ") == -1
			|| find_plyr_pos(data, tab[i], i - map_start - 1, &plyr_bool))
			return (-1);
	}
	if (!plyr_bool && printf("Error\nPlayer not found\n"))
		return (-1);
	if ((data->raycast.floor_color == -1 || data->raycast.ceiling_color == -1)
		&& printf("Error\nOne of the colors is not initialized\n"))
		return (-1);
	return (0);
}
