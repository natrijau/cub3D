#include "cub3d.h"

int	map_len(char *file)
{
	int		fd;
	char	*line;
	int		len;

	fd = open(file, O_RDONLY);
	if (fd < 0 && printf("Error\nCould not open file.\n"))
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

void	map_clear(char **map)
{
	int	i;

	i = 0;
	if (!map)
		return ;
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
