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
	int	 fd;
	char	**map;
	char	*line;
	int	 i;

	fd = open(file, O_RDONLY);
	//! printf("Error\nCould not open file.\n");
	map = malloc(sizeof(char *) * (map_len(file) + 1));
	if (!map)
	{
		close(fd);
		return (NULL);
	}
	i = 0;
	line = get_next_line(fd);
	map[i] = line;
	while (line)
	{
		map[i][ft_strlen(map[i]) - 1] = '\0';
		line = get_next_line(fd);
		map[++i] = line;
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
    int i = ft_strtablen(file_content) - 1;
    while (i >= 0)
    {
        if (!is_map_line(file_content[i]))
            return (i);
        i--;
    }
    return (-1);
}

void	init_direction(t_data *data)
{
	data->path_wall = ft_calloc(sizeof(char *), 4);
	data->compass = ft_calloc(sizeof(char *), 4);
	data->compass[0] = ft_strdup("NO");
	data->compass[1] = ft_strdup("SO");
	data->compass[2] = ft_strdup("WE");
	data->compass[3] = ft_strdup("EA");
}

int	get_file_texture(t_data *data, char *str)
{
	int		i;
	int		fd;

	i = 0;
	fd = 0;
	if (str[0] == 'F' || str[0] == 'C')
		return (0);	
	while (i < 4)
	{
		if (!ft_strncmp(str, data->compass[i], 2))
			break;
		i++;
	}
	fd = open(&str[2], O_RDONLY);
	if (fd < 0)
		return (-1);
	if (data->path_wall[i] != NULL)
	{
		// Valeur deja remplie
		//! Renvoyer une erreur precise ?
		return (-1);
	}			
	data->path_wall[i] = ft_strdup(&str[2]);
	close(fd);
	return (0);
}

int	check_color(t_data *data, char *str)
{
	(void) data;
	if (str[0] == 'F')
	{
		//! determiner la couleurs en fonction des trois valeurs cles RGB, tester les limites de ces valeurs
		return (0);
	}
	else if (str[0] == 'C')
	{
		//! determiner la couleurs en fonction des trois valeurs cles RGB, tester les limites de ces valeurs
		return (0);
	}
	return (0);
}

int	not_valid_str(t_data *data, char *str)
{
	int		i;

	i = 0;
	while (i <= 5)
	{
		if (i < 4 && !ft_strncmp(str, data->compass[i], 2))
			break;
		i++;
	}
	if (i == 5)
		return (-1);		
	return (0);
}

int	valid_textures(t_data *data, char **tab)
{
	init_direction(data);
	int i;

	i = 0;
	while (tab[i])
	{
		if (check_color(data, tab[i]))
			return (-1);		
		else if (get_file_texture(data, tab[i]))
			return (-1);
		else if (not_valid_str(data, tab[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	check_textures_colors(t_data *data, char **tab)
{
	int	i;
	(void)data;

	i = 0;

	if (ft_strtablen(tab) > 6 || ft_strtablen(tab) < 4)
	{
		//!perror("erreur nombre d'arguments");
		return (-1);		
	}
	while (tab[i])
	{
		tab[i] = clear_space(tab[i]);
		i++;
	}
	if (valid_textures(data, tab))
		return (-1);
	return (0);
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

char **remove_empty_lines(char **lines, int num_lines)
{
	int		count;
	char	**new_lines;
	int		i;
	int		j;

	i = 0;
	j = 0;
	count = 0;
	while (i < num_lines)
	{
		if (!is_empty_line(lines[i])){
			count++;
		}
		i++;
	}
	new_lines = malloc(count * sizeof(char *));
	if (!new_lines) {
		//?perror("malloc failed");
		//return error;
		// return ;
	}
	i = 0;
	while (i < num_lines)
	{
		if (!is_empty_line(lines[i]))
		{
			new_lines[j] = &lines[i][0];
			j++;
		}
		i++;
	}
	return (new_lines);
}


// Validate map
int	parsing(t_data *data, char *file)
{
	(void) data;
	char	**file_content;
	char	**content_except_map;
	char	**map_off;
	(void) content_except_map;
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
	content_except_map = remove_empty_lines(file_content, map_start);

	if (check_textures_colors(data, content_except_map))
	{
		printf("Error\n");
		//? free mapp_off
		return (-1); 
	}	
	map_off = tab_cpy(get_map(file_content)); // Charging map since file content
	// map_off = &file_content[map_start + 1];
	if (!map_off || init_start(map_off, data) == -1)  // valid map ?
	{
		printf("Error\n");
		return (-1); 
	}
	data->map = init_map(map_off);
	if (!data->map || pars_map(map_off, data->map, data->y / CASE, data->x / CASE) == -1)
	{
		printf("Error\n");
		map_clear(map_off);
		if (data->map)
			map_clear(data->map);
		return (-1);
	}
	printf("Map:\n");
	map_clear(map_off);
	return (0);
}
