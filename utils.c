#include "cub3d.h"

void free_door_tab(int **tab_door)
{
	int i = 0;

	if (!tab_door)
		return;
	while (tab_door && tab_door[i])
	{
		free(tab_door[i]);
		i++;
	}
	free(tab_door);
}


int	ft_strtablen(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		++i;
	return (i);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '	' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

int	is_empty_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

// returns the length of the longest line on map
int	get_max_tab_len(char **map)
{
	int	max_len;
	int	tmp_len;
	int	i;

	i = 1;
	tmp_len = 0;
	max_len = ft_strlen(map[0]);
	while (map[i])
	{
		tmp_len = ft_strlen(map[i]);
		if (tmp_len > max_len)
			max_len = tmp_len;
		i++;
	}
	return (max_len);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '	' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

int	is_empty_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

// returns the length of the longest line on map
int	get_max_tab_len(char **map)
{
	int	max_len;
	int	tmp_len;
	int	i;

	i = 1;
	tmp_len = 0;
	max_len = ft_strlen(map[0]);
	while (map[i])
	{
		tmp_len = ft_strlen(map[i]);
		if (tmp_len > max_len)
			max_len = tmp_len;
		i++;
	}
	return (max_len);
}
