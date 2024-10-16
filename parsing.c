#include "cub3d.h"

// returns the length of the longest line on map
int		get_max_tab_len(char **map)
{
	int	max_len;
	int	tmp_len;
	int	i;

	i = 0;
	tmp_len = 0;
	max_len = ft_strlen(map[i]);
	while (map[i])
	{
		tmp_len = ft_strlen(map[i]);
		if (tmp_len > max_len)
			max_len = tmp_len;		
		i++;
	}
	return (max_len);
}

//  Read map since file and return tab map
char	**get_map(char *file)
{
	int	 fd;
	char	**map;
	char	*line;
	int	 i;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (NULL);
	map = malloc(sizeof(char *) * (map_len(file) + 1));
	if (!map)
	{
		close(fd);
		return (NULL);
	}
	i = 0;
	line = get_next_line(fd);
	map[i] = line;
	while (line)
	{
		map[i][ft_strlen(map[i]) - 1] = '\0';
		line = get_next_line(fd);
		map[++i] = line;
	}
	close(fd);
	return (map);
}

// Add space to map to equalize the length of the lines
char	**init_map(char **map_off)
{
	char	**map;
	int		i;
	int		j;
	int		max_len;

	max_len = get_max_tab_len(map_off);  // max line
	map = malloc(sizeof(char *) * (ft_strtablen(map_off) + 1));
	if (!map)
		return (NULL);
	i = -1;
	while (map_off[++i])
	{
		map[i] = malloc(sizeof(char) * (max_len + 1));
		if (!map[i])
		{
			map_clear(map);
			return (NULL);
		}
		j = -1;
		while (++j < max_len)
			map[i][j] = '1';  // Fill with '1' to avoid out of bounds
		map[i][j] = '\0';
	}
	map[i] = NULL;
	return (map);
}

// Recursive parsing function to validate and format map
int	pars_map(char **map, char **space, int k, int i)
{
	if (k == 0 || i == 0 || k >= ft_strtablen(map) - 1
		|| i >= ft_strlen(map[k]) - 1)  // Check map limits
	{
		map_clear(map);
		map_clear(space);
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	space[k][i] = map[k][i];  // Copy map to space
	map[k][i] = '1';  // Mark current position as '1'
	/* 
	print_map(map, TRUE); 
	print_map(space, TRUE); 
	*/

	// Recursive calls to continue parsing in all directions
	if (map[k][i + 1] && map[k][i + 1] != '1')
		pars_map(map, space, k, i + 1);
	if (map[k + 1] && map[k + 1][i] && map[k + 1][i] != '1')
		pars_map(map, space, k + 1, i);
	if (map[k][i - 1] && map[k][i - 1] != '1')
		pars_map(map, space, k, i - 1);
	if (map[k - 1][i] && map[k - 1][i] != '1')
		pars_map(map, space, k - 1, i);
	return (0);
}

// Validate map
int	parsing(t_data *data, char *file)
{
	char	**map_off;  

	map_off = get_map(file);  // Charging map since file
	if (!map_off || init_start(map_off, data) == -1)  // valid map ?
	{
		printf("Error\n");
		return (-1); 
	}
	data->map = init_map(map_off);
	if (!data->map || pars_map(map_off, data->map, data->y / CASE, data->x / CASE) == -1)
	{
		printf("Error\n");
		map_clear(map_off);
		if (data->map)
			map_clear(data->map);
		return (-1);
	}
	printf("Map:\n");
	map_clear(map_off);
	return (0);
}
