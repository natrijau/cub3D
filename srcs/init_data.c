/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:59:00 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 12:59:00 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int	init_texture(t_data *data, char *str)
{
	if (str[0] == 'F' || str[0] == 'C')
		return (0);
	if (ft_strncmp(str, "NO", 2) && ft_strncmp(str, "SO", 2)
		&& ft_strncmp(str, "WE", 2) && ft_strncmp(str, "EA", 2))
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
			data->first_rayangle = (data->fov_rad - M_PI / 2.0) / 2.0;
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
		printf("Error\nInvalid element information\n");
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

	if (error_map_start(map_start, tab))
		return (-1);
	plyr_bool = FALSE;
	i = -1;
	map_start--;
	while (tab[++i])
	{
		if (i <= map_start)
		{
			tab[i] = clear_space(tab[i]);
			if (init_color(data, tab[i])
				|| init_texture(data, tab[i]))
				return (-1);
		}
		else if (is_map_line(tab[i], "01NSEW ") == -1
			|| find_plyr_pos(data, tab[i], i - map_start - 1, &plyr_bool))
			return (-1);
	}
	if ((data->raycast.floor_color == -1 || data->raycast.ceiling_color == -1)
		&& printf("Error\nOne of the colors is not initialized\n"))
		return (-1);
	return (0);
}
