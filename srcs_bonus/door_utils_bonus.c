/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:49:27 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 12:49:28 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	find_door_in_map(char **str)
{
	int	i;
	int	j;
	int	d;

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
	return (d);
}

int	malloc_door_tab(int	**tab_door, char **str)
{
	int	i;
	int	j;
	int	d;

	i = 0;
	d = -1;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == 'D')
			{
				tab_door[++d] = malloc(sizeof(int) * 2);
				if (!tab_door[d])
					return (-1);
				tab_door[d][0] = j;
				tab_door[d][1] = i;
			}
			j++;
		}
		i++;
	}
	tab_door[++d] = NULL;
	return (0);
}

int	**init_door_tab(char **map)
{
	int	**tab_door;

	tab_door = malloc(sizeof(int *) * (find_door_in_map(map) + 1));
	if (!tab_door)
		return (NULL);
	if (malloc_door_tab(tab_door, map) == -1)
	{
		free_door_tab(tab_door);
		return (NULL);
	}
	return (tab_door);
}
