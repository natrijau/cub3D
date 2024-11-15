#include "cub3d.h"

//  Read map since file and return tab map
static char	**get_file(char *file)
{
	char	**map;
	int		fd;
	int		i;

	fd = open(file, O_RDONLY);
	if (fd < 0 && printf("Error\nInvalid file type\n"))
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
	if (data->raycast.N_wall.img == NULL || data->raycast.S_wall.img == NULL
		|| data->raycast.W_wall.img == NULL || data->raycast.E_wall.img == NULL)
	{
		printf("Error\nOne of the textures is not initialized\n");
		return (-1);
	}
	return (0);
}

int	init(t_data *data, char **file, int map_start)
{
	if (init_data(data, file, map_start) == -1)
	{
		map_clear(file);
		return (-1);
	}
	if ((data->raycast.floor_color && !data->raycast.ceiling_color)
		|| (!data->raycast.floor_color && data->raycast.ceiling_color))
	{
		printf("Error\nOne of the colors in the month have not been defined\n");
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
	if (flood_fil(&file[map_start], data->map, data->y / CASE, data->x / CASE))
	{
		printf("Error\nInvalid map\n");
		map_clear(file);
		return (-1);
	}
	data->tab_door = init_door_tab(data->map);
	map_clear(file);
	if (check_texture(data) == -1)
		return (-1);
	return (0);
}
