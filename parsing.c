#include "cub3d.h"

// returns the length of the longest line on map
int		get_max_tab_len(char **map)
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

//  Read map since file and return tab map
char	**get_file(char *file)
{
	int		fd;
	char	**map;
	char	*line;
	int		i;

	fd = open(file, O_RDONLY);
	map = ft_calloc(sizeof(char *), (map_len(file) + 1));
	if (!map || fd < 0)
	{
		close(fd);
		return (NULL);
	}
	i = 0;
	while (TRUE)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (is_empty_line(line))
		{
			free(line);
			continue ;
		}
		map[i] = line;
		map[i][ft_strlen(map[i]) - 1] = '\0';
		i++;
	}
	close(fd);
	return (map);
}

// Add space to map to equalize the length of the lines
char	**init_map(char **map_off)
{
	char	**map;
	int		i;
	int		j;
	int		max_len;

	max_len = get_max_tab_len(map_off);  // max line
	map = malloc(sizeof(char *) * (ft_strtablen(map_off) + 1));
	if (!map)
		return (NULL);
	i = -1;
	while (map_off[++i])
	{
		map[i] = malloc(sizeof(char) * (max_len + 1));
		if (!map[i])
		{
			map_clear(map);
			return (NULL);
		}
		j = -1;
		while (++j < max_len)
			map[i][j] = '1';  // Fill with '1' to avoid out of bounds
		map[i][j] = '\0';
	}
	map[i] = NULL;
	return (map);
}

// Recursive parsing function to validate and format map
int	pars_map(char **map, char **space, int k, int i)
{
	if ((k == 0 || i == 0 || k >= ft_strtablen(map) - 1
		|| i >= ft_strlen(map[k]) - 1) || !ft_strchr("01P ", map[k][i]))  // Check map limits
	{
		map_clear(map);
		map_clear(space);
		printf("Error\n");
		exit(EXIT_FAILURE);
	}
	
	space[k][i] = map[k][i];  // Copy map to space
	map[k][i] = '1';  // Mark current position as '1'
	/* 
	print_map(map, TRUE); 
	print_map(space, TRUE); 
	*/
	// Recursive calls to continue parsing in all directions
	if (map[k][i + 1] && map[k][i + 1] != '1')
		pars_map(map, space, k, i + 1);
	if (map[k + 1] && map[k + 1][i] && map[k + 1][i] != '1')
		pars_map(map, space, k + 1, i);
	if (map[k][i - 1] && map[k][i - 1] != '1')
		pars_map(map, space, k, i - 1);
	if (map[k - 1][i] && map[k - 1][i] != '1')
		pars_map(map, space, k - 1, i);
	return (0);
}

// Debut de la map en partant du bas
int find_map_start(char **file_content)
{
	int i;

	i = ft_strtablen(file_content) - 1;
	while (i >= 0)
	{
		if (!is_map_line(file_content[i], "01NSEW \n"))
		{
			if (i > 6 || i < 4)
				return (-1);
			return (i);
		}
		i--;
	}
	return (-1);
}

int	add_direction_img(t_image *dest, t_image *src, char *str, char *direction)
{
	if (!ft_strncmp(str, direction, 2))
	{
		if (!dest)
			return (-1);
		*dest = *src;
	}
	return (0);
}

int	get_file_texture(t_data *data, char *str)
{
	t_image	img;

	if (str[0] == 'F' || str[0] == 'C')
		return (0);
	img = get_wall(data->mlx, &str[2]);
	if (!img.img)
		return (-1);
	if (add_direction_img(&data->raycast.N_wall, &img, str, "NO") == -1)
		return (-1);
	if (add_direction_img(&data->raycast.S_wall, &img, str, "SO") == -1)
		return (-1);
	if (add_direction_img(&data->raycast.W_wall, &img, str, "WE") == -1)
		return (-1);
	if (add_direction_img(&data->raycast.E_wall, &img, str, "EA") == -1)
		return (-1);
	return (0);
}

int	check_color(t_data *data, char *str)
{
	(void)data;
	if (str[0] == 'F')
	{

		return (0);
	}
	else if (str[0] == 'C')
	{
		return (0);
	}
	return (0);
}

int	valid_textures(t_data *data, char **tab, int map_start)
{
	int i;

	i = 0;
	while (i < map_start)
	{
		if (check_color(data, tab[i]))
			return (-1);		
		else if (get_file_texture(data, tab[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	check_textures_colors(t_data *data, char **tab, int map_start)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		tab[i] = clear_space(tab[i]);
		i++;
	}
	if (valid_textures(data, tab, map_start))
		return (-1);
	return (0);
}

// Validate map
int	parsing(t_data *data, char *file)
{
	char	**file_content;
	int		map_start;

	file_content = get_file(file);  // Charging all file content
	map_start = find_map_start(file_content);
	if (map_start == -1)
	{
		//!printf("Error: No valid map found.\n");
		printf("Error\n");
		map_clear(file_content);
		return (-1);
	}
	if (check_textures_colors(data, file_content, map_start))
	{
		printf("Error\n");
		//? free mapp_off
		return (-1); 
	}
	// printf("&file_content[map_start] %s \n", &file_content[map_start]);
	if (init_start(&file_content[map_start], data) == -1)  // valid map ?
	{
		printf("Error\n");
		return (-1); 
	}
	data->map = init_map(&file_content[map_start]);
	if (!data->map || pars_map(&file_content[map_start], data->map, data->y / CASE, data->x / CASE) == -1)
	{
		printf("Error\n");
		map_clear(file_content);
		if (data->map)
			map_clear(data->map);
		return (-1);
	}
	printf("Map:\n");
	map_clear(file_content);
	return (0);
}
