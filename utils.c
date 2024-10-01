#include "cub3d.h"

int  map_len(char *file)
{
    int     fd;
    char    *line;
    int     len;

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

int	get_start_xy(char **map, int *x, int *y)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_strchr("NSEW", map[i][j]))
			{
				*x = i;
				*y = j;
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
