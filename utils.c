#include "cub3d.h"

int  map_len(char *file)
{
	int	 fd;
	char	*line;
	int	 len;

	fd = open(file, O_RDONLY);
	//!printf("Error\nCould not open file.\n");
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
		return (N);
	if (direction == 'E')  // East
		return (E);
	if (direction == 'S')  // south
		return (S);
	if (direction == 'W')  // west
		return (W);
	return (-1);
}

void	find_player_position(t_data *data, char **map, int line, int *bool)
{
	int	col;

	col = 0;
	while (map[line][col])
	{
		if (ft_strchr("NSEW", map[line][col]))
		{
			data->x = col * CASE + CASE / 2;  // Player's horizontal position
			data->y = (line + 1) * CASE + CASE / 2;  // Player's vertical position
			data->angle = get_angle(map[line][col]);  // Initializes the angle based on the direction found
			data->fov_rad = (FOV * M_PI) / 180;  // vision converted to radians
			map[line][col] = '0';
			(*bool)++;
		}
		col++;
	}
}

// initialize player position on map
int init_start(char **map, t_data *data)
{
	int i;
	int find_player;

	i = 0;
	find_player = 0;
	if (!map)
		return (-1);
	while (map[i])
	{
		find_player_position(data, map, i, &find_player);
		++i;
	}
	if (find_player != 1)
	{
		printf("Invalid numb of player\n");
		return (-1);		
	}	
	return (0);
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

char    **tab_cpy(char **tab)
{
	int		i;
	char	**cpy;

	i = 0;
	cpy = ft_calloc(sizeof(char *) , ft_strtablen(tab) + 1);
	while (tab[i])
	{
		cpy[i] = ft_strdup(tab[i]);
		i++;
	}
	cpy[i++] = NULL;
	return (cpy);
}

int	count_char(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;		
	}
	return (count);
}

//Withat malloc. replace value str after dell alloc str
char	*copy_without_char(char *cpy, char *src, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] != c && src[i])
		{
			cpy[j] = src[i];
			j++;
		}
		i++;
	}
	return (cpy);		
}

char	*clear_space(char *str)
{
	char 	*cpy;
	int	i;
	int	j;

	i = 0;
	j = 0;

	if (count_char(str, ' ') == 0)
		return (str);	
	cpy = ft_calloc(sizeof(char), (ft_strlen(str) - count_char(str, ' ')) + 1);
	//!verif calloc
	if (!cpy)
		return (NULL);	
	while (str[i])
	{
		if (str[i] != ' ' && str[i])
		{
			cpy[j] = str[i];
			j++;
		}
		i++;
	}
	free(str);
	return (cpy);
}

int is_map_line(char *line_map, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line_map[i])
	{
		while (str[j])
		{
			if (str[j] == line_map[i])
			{
				j = 0;
				break;
			}
			j++;
		}
		if (str[j] == '\0')
			return (0);		
		i++;
	}
	return (1);
}

int ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')
		return (1);
	return (0);
}

int is_empty_line(const char *line)
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

	i = 0;
	tmp_len = 0;
	max_len = ft_strlen(map[i]);
	while (map[i])
	{
		tmp_len = ft_strlen(map[i]);
		if (tmp_len > max_len)
			max_len = tmp_len;
		i++;
	}
	return (max_len);
}
