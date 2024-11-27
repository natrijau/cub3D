#include "cub3d.h"

int	check_zero_inside(char **tab, int start)
{
	int	i;
	int	j;

	i = start + 1;
	j = 1;
	while (tab[i])
	{
		while (tab[i][j + 1])
		{
			if (tab[i][j] == '0')
			{
				if (!ft_strchr("01", tab[i - 1][j])
					|| !ft_strchr("01", tab[i + 1][j]))
					return (-1);
				if (!ft_strchr("01", tab[i][j + 1])
					|| !ft_strchr("01", tab[i][j - 1]))
					return (-1);
			}
			j++;
		}
		j = 1;
		i++;
	}
	return (0);
}

int	valid_zero_map(int start, char **tab)
{
	int	i;

	i = start;
	if (ft_strchr(tab[start], '0') || ft_strchr(tab[ft_strtablen(tab)], '0'))
		return (-1);
	i++;
	while (tab[i])
	{
		if (tab[i][0] == '0' || tab[i][ft_strlen(tab[i]) - 1] == '0')
			return (-1);
		i++;
	}
	if (check_zero_inside(tab, start) == -1)
		return (-1);
	return (0);
}

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
int	flood_fil(t_data *data, char **map, int x, int i)
{
	if ((!x || !i || x >= ft_strtablen(map) - 1
			|| i >= ft_strlen(map[x]) - 1 || map[x][i] == ' '))
		return (-1);
	data->map[x][i] = map[x][i];
	map[x][i] = '1';
	if (map[x][i + 1] && map[x][i + 1] != '1'
		&& flood_fil(data, map, x, i + 1))
		return (-1);
	if (map[x + 1][i] && map[x + 1][i] != '1'
		&& flood_fil(data, map, x + 1, i))
		return (-1);
	if (map[x][i - 1] && map[x][i - 1] != '1'
		&& flood_fil(data, map, x, i - 1))
		return (-1);
	if (map[x - 1][i] && map[x - 1][i] != '1'
		&& flood_fil(data, map, x - 1, i))
		return (-1);
	return (0);
}
