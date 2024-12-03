/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:51:57 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 12:51:58 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
				|| !ft_strchr("D01", tab[i - 1][j]) || !tab[i + 1]
				|| j >= ft_strlen(tab[i + 1])
				|| !ft_strchr("D01", tab[i + 1][j])
				|| !ft_strchr("D01", tab[i][j - 1])
				|| !ft_strchr("D01", tab[i][j + 1])))
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

// Add space to map to equalize the length of the lines
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

// // Recursive parsing function to validate and format map
// int	flood_fil(t_data *data, char **map, int x, int i)
// {
// 	if ((!x || !i || x >= ft_strtablen(map) - 1
// 			|| i >= ft_strlen(map[x]) - 1 || map[x][i] == ' '))
// 		return (-1);
// 	if (map[x][i] == 'D' && !data->raycast.door.img)
// 		map[x][i] = '0';
// 	data->map[x][i] = map[x][i];
// 	map[x][i] = '1';
// 	if (map[x][i + 1] && map[x][i + 1] != '1'
// 		&& flood_fil(data, map, x, i + 1))
// 		return (-1);
// 	if (map[x + 1][i] && map[x + 1][i] != '1'
// 		&& flood_fil(data, map, x + 1, i))
// 		return (-1);
// 	if (map[x][i - 1] && map[x][i - 1] != '1'
// 		&& flood_fil(data, map, x, i - 1))
// 		return (-1);
// 	if (map[x - 1][i] && map[x - 1][i] != '1'
// 		&& flood_fil(data, map, x - 1, i))
// 		return (-1);
// 	return (0);
// }
