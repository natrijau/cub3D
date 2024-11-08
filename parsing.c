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

// Debut de la map en partant du bas
static int	find_map_start(char **file_content)
{
	int	i;
	int	j;

	i = ft_strtablen(file_content) - 1;
	while (i >= 4)
	{
		j = 0;
		while (file_content[i][j] && ft_strchr("01NSEW ", file_content[i][j]))
			j++;
		if (file_content[i][j] && i <= 6)
			return (i);
		else if (file_content[i][j])
			break ;
		i--;
	}
	return (-1);
}

// Add space to map to equalize the length of the lines
static char	**init_map(char **map_off)
{
	char	**map;
	int		i;
	int		j;
	int		max_len;

	map = ft_calloc(sizeof(char *), (ft_strtablen(map_off) + 1));
	if (!map)
		return (NULL);
	i = -1;
	max_len = get_max_tab_len(map_off);
	while (map_off[++i])
	{
		map[i] = ft_calloc(sizeof(char), (max_len + 1));
		if (!map[i])
		{
			map_clear(map);
			return (NULL);
		}
		j = -1;
		while (++j < max_len)
			map[i][j] = '1';
	}
	return (map);
}

// Recursive parsing function to validate and format map
static int	flood_fil(char **map, char **space, int x, int i)
{
	if ((!x || !i || x >= ft_strtablen(map) - 1 || i >= ft_strlen(map[x]) - 1
		|| map[x][i] == ' '))
			return (-1);
	space[x][i] = map[x][i];
	map[x][i] = '1';
	if (map[x][i + 1] && map[x][i + 1] != '1' && flood_fil(map, space, x, i + 1))
		return (-1);
	if (map[x + 1][i] && map[x + 1][i] != '1' && flood_fil(map, space, x + 1, i))
		return (-1);
	if (map[x][i - 1] && map[x][i - 1] != '1' && flood_fil(map, space, x, i - 1))
		return (-1);
	if (map[x - 1][i] && map[x - 1][i] != '1' && flood_fil(map, space, x - 1, i))
		return (-1);
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
	map_clear(file);
	return (0);
}
