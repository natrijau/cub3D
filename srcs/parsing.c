/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:59:46 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 14:37:02 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//  Read map since file and return tab map
char	**get_file(char *file)
{
	char	**map;
	int		fd;
	int		i;

	fd = open(file, O_RDONLY);
	if (fd < 0 && printf("Error\nInvalid file\n"))
		return (NULL);
	map = malloc(sizeof(char *) * (map_len(file) + 1));
	i = 0;
	while (map)
	{
		map[i] = get_next_line(fd);
		if (!map[i])
			break ;
		if (is_empty_line(map[i]))
		{
			free(map[i]);
			continue ;
		}
		if (map[i][ft_strlen(map[i]) - 1] == '\n')
			map[i][ft_strlen(map[i]) - 1] = '\0';
		i++;
	}
	close(fd);
	return (map);
}

int	check_texture(t_data *data)
{
	if (!data->raycast.n_wall.img || !data->raycast.s_wall.img
		|| !data->raycast.w_wall.img || !data->raycast.e_wall.img)
	{
		printf("Error\nOne of the textures is not initialized\n");
		return (-1);
	}
	return (0);
}

int	init(t_data *data, char **file, int map_start)
{
	if (error_map_start(map_start, file)
		|| init_data(data, file, map_start - 1) == -1)
	{
		map_clear(file);
		return (-1);
	}
	data->map = init_map(&file[map_start]);
	if (!data->map)
	{
		map_clear(file);
		return (-1);
	}
	return (0);
}

// Validate map
int	parsing(t_data *data, char *path_file)
{
	char	**file;
	int		map_start;

	file = get_file(path_file);
	if (!file)
		return (-1);
	map_start = find_map_start(file) + 1;
	if (init(data, file, map_start) == -1)
		return (-1);
	map_clear(file);
	if (valid_zero_map(data->map))
	{
		printf("Error\nInvalid map\n");
		return (-1);
	}
	if (check_texture(data) == -1)
		return (-1);
	return (0);
}
