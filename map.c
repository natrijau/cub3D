#include "cub3d.h"

// Add space to map to equalize the length of the lines
char	**init_map(char **map_off)
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
int	flood_fil(char **map, char **space, int x, int i)
{
	if ((!x || !i || x >= ft_strtablen(map) - 1 || i >= ft_strlen(map[x]) - 1
			|| map[x][i] == ' '))
		return (-1);
	space[x][i] = map[x][i];
	map[x][i] = '1';
	if (map[x][i + 1] && map[x][i + 1] != '1'
		&& flood_fil(map, space, x, i + 1))
		return (-1);
	if (map[x + 1][i] && map[x + 1][i] != '1'
		&& flood_fil(map, space, x + 1, i))
		return (-1);
	if (map[x][i - 1] && map[x][i - 1] != '1'
		&& flood_fil(map, space, x, i - 1))
		return (-1);
	if (map[x - 1][i] && map[x - 1][i] != '1'
		&& flood_fil(map, space, x - 1, i))
		return (-1);
	return (0);
}