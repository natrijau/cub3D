#include "cub3d.h"

int	player_case(t_data *data)
{
	int	i;

	i = 0;
	if (!data->tab_door)
		return (0);
	while (data->tab_door && data->tab_door[i])
	{
		if (data->tab_door[i][0] == (int)(data->y / CASE)
			&& data->tab_door[i][1] == (int)(data->x / CASE))
			return (-1);
		i++;
	}
	return (0);
}

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

int	**malloc_door_tab(int	**tab_door, char **str, int i, int j)
{
	int	d;

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

int	**init_door_tab(char **str)
{
	int	i;
	int	j;
	int	d;
	int	**tab_door;

	i = 0;
	j = 0;
	d = find_door_in_map(str);
	tab_door = malloc(sizeof(int *) * (d + 1));
	if (!tab_door)
		return (NULL);
	tab_door[d] = NULL;
	tab_door = malloc_door_tab(tab_door, str, i, j);
	return (tab_door);
}
