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

// returns the starting angle based on the direction
double get_angle(char direction)
{
    if (direction == 'N')  // north
        return (2 * M_PI);
    if (direction == 'E')  // East
        return (3 * M_PI / 2);
    if (direction == 'S')  // south
        return (M_PI);
    if (direction == 'W')  // west
        return (M_PI / 2);
    return (-1);
}


// initialize player position on map
int init_start(char **map, t_data *data)
{
    int i;
    int j;

    i = 0;
    if (!map)
        return (-1);

    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (ft_strchr("NSEW", map[i][j]))  // Finds the player's position based on his direction
            {
                data->x = j * CASE + CASE / 2;  // Player's horizontal position
                data->y = i * CASE + CASE / 2;  // Player's vertical position
                data->angle = get_angle(map[i][j]);  // Initializes the angle based on the direction found
                data->fov_rad = (FOV * M_PI) / 180;  // vision converted to radians
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
