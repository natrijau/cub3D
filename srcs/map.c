#include "cub3d.h"

int	check_zero_inside(char **tab)
{
	int	i;
	int	j;

	i = 1;
	while (tab[i])
	{
		j = 1;
		while (tab[i][j + 1])
		{
			if (tab[i][j] == '0' && (j >= ft_strlen(tab[i - 1])
				|| !ft_strchr("01", tab[i - 1][j]) || !tab[i + 1]
				|| j >= ft_strlen(tab[i + 1]) || !ft_strchr("01", tab[i + 1][j])
				|| !ft_strchr("01", tab[i][j - 1])
				|| !ft_strchr("01", tab[i][j + 1])))
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

int	valid_zero_map(char **tab)
{
	int	i;

	i = 0;
	if (ft_strchr(tab[i], '0') || ft_strchr(tab[ft_strtablen(tab)], '0'))
		return (-1);
	i++;
	while (tab[i])
	{
		if (tab[i][0] == '0' || tab[i][ft_strlen(tab[i]) - 1] == '0')
			return (-1);
		i++;
	}
	if (check_zero_inside(tab) == -1)
		return (-1);
	return (0);
}

char	**init_map(char **map_off)
{
	char	**map;
	int		i;

	map = ft_calloc(sizeof(char *), (ft_strtablen(map_off) + 1));
	if (!map)
		return (NULL);
	i = -1;
	while (map_off[++i])
		map[i] = map_off[i];
	map_off[0] = NULL;
	return (map);
}
