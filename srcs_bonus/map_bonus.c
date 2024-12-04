/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:51:57 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/04 14:01:01 by yanolive         ###   ########.fr       */
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
			if ((tab[i][j] == '0' || tab[i][j] == 'D')
				&& (j >= ft_strlen(tab[i - 1])
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
	if (ft_strchr(tab[i], '0') || ft_strchr(tab[ft_strtablen(tab)], '0')
		|| ft_strchr(tab[i], 'D') || ft_strchr(tab[ft_strtablen(tab)], 'D'))
		return (-1);
	i++;
	while (tab[i])
	{
		if (tab[i][0] == '0' || tab[i][ft_strlen(tab[i]) - 1] == '0'
			|| tab[i][0] == 'D' || tab[i][ft_strlen(tab[i]) - 1] == 'D')
			return (-1);
		i++;
	}
	if (check_zero_inside(tab) == -1)
		return (-1);
	return (0);
}

// Add space to map to equalize the length of the lines
char	**init_map(t_data *data, char **map_off)
{
	char	**map;
	int		i;
	int		j;

	map = ft_calloc(sizeof(char *), (ft_strtablen(map_off) + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (map_off[i])
	{
		j = 0;
		while (!data->raycast.door.img && map_off[i][j])
		{
			if (map_off[i][j] == 'D')
				map_off[i][j] = '0';
			++j;
		}
		map[i] = map_off[i];
		++i;
	}
	map_off[0] = NULL;
	return (map);
}
