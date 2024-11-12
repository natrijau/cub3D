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

int	**init_door_tab(char **str)
{
	int	i;
	int	j;
	int	d;
	int	**tab_door;

	i = 0;
	d = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == 'D')
				d++;
			j++;
		}
		i++;
	}
	tab_door = malloc(sizeof(int *) * (d + 1));
	if (!tab_door)
		return (NULL);
	tab_door[d] = NULL;
	i = 0;
	d = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == 'D')
			{
				tab_door[d] = malloc(sizeof(int) * 2);
				if (!tab_door[d])
					return (NULL);
				tab_door[d][0] = i;
				tab_door[d][1] = j;
				d++;
			}
			j++;
		}
		i++;
	}
	return (tab_door);
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
	if (init_data(data, file, map_start) == -1)
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
	if (flood_fil(&file[map_start], data->map, data->y / CASE, data->x / CASE))
	{
		printf("Error\nInvalid map\n");
		map_clear(file);
		return (-1);
	}
	data->tab_door = init_door_tab(data->map);
	map_clear(file);
	return (0);
}
