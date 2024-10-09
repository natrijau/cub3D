#include "cub3d.h"

int  map_len(char *file)
{
	int	 fd;
	char	*line;
	int	 len;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1);
	len = 0;
	line = get_next_line(fd);
	while (line)
	{
		len++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (len);
}

double	get_angle(char direction)
{
	if (direction == 'N')
		return (2 * M_PI);
	if (direction == 'E')
		return (3 * M_PI / 2);
	if (direction == 'S')
		return (M_PI);
	if (direction == 'W')
		return (M_PI / 2);
	return (-1);
}

int	init_start(char **map, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	if (!map)
		return (-1);
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_strchr("NSEW", map[i][j]))
			{
				data->x = j * CASE + CASE / 2;
				data->y = i * CASE + CASE / 2;
				data->angle = get_angle(map[i][j]);
				data->fov_rad = (FOV * M_PI) / 180;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

void	map_clear(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		free(map[i++]);
	free(map);
}

int	ft_strtablen(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		++i;
	return (i);
}
