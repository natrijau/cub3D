#include "cub3d.h"

static int	init_color(t_data *data, char *str)
{
	int		color;
	char	**tab;

	color = 0;
	if (str[0] != 'F' && str[0] != 'C')
		return (0);
	tab = ft_split(&str[1], ',');
	if (check_color_value(tab))
	{
		map_clear(tab);
        printf("Error\nInvalid color value\n");
		return (-1);
	}
	color = (ft_atoi(tab[0]) << 16) | (ft_atoi(tab[1]) << 8) | ft_atoi(tab[2]);
	if (str[0] == 'F' && !data->raycast.floor_color)
		data->raycast.floor_color = color;
	else if (str[0] == 'C' && !data->raycast.ceiling_color)
		data->raycast.ceiling_color = color;
	else if (printf("Error\nDouble color definition\n"))
		return (-1);
	map_clear(tab);
	return (0);
}

static int	init_texture(t_data *data, char *str)
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

static int is_map_line(char *line_map, char *str)
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
		if (str[j] == '\0' && printf("Error\nInvalid map line\n"))
			return (0);
		i++;
	}
	return (1);
}

static int	find_plyr_pos(t_data *data, char *line, int y, int *find_plyr)
{
	int	x;

	x = 0;
	while (line[x])
	{
		if (ft_strchr("NSEW", line[x]))
		{
			if (*find_plyr)
			{
				printf("Error\nMultiple player positions\n");
				return (-1);
			}
			data->x = ((double)x + 0.5) * CASE;  // Player's horizontal position
			data->y = ((double)y + 0.5) * CASE;  // Player's vertical position
			data->angle = get_angle(line[x]);  // Initializes the angle based on the direction found
			data->fov_rad = (FOV * M_PI) / 180;  // vision converted to radians
			data->first_rayangle = (data->fov_rad / 2.0) - M_PI / 4.0;
			data->angle_step = data->fov_rad / WIDTH;
			line[x] = '0';
			*find_plyr = TRUE;
		}
		x++;
	}
	return (0);
}

int	init_data(t_data *data, char **tab, int map_start)
{
	int	i;
	int	plyr_bool;

	if (--map_start == -1 && printf("Error\nInvalid element information"))
		return (-1);
	data->raycast.floor_color = 0;
	data->raycast.ceiling_color = 0;
	plyr_bool = FALSE;
	i = -1;
	while (tab[++i])
	{
		if (i <= map_start)
		{
			tab[i] = clear_space(tab[i]);
			if (init_color(data, tab[i])
				|| init_texture(data, tab[i]))
				return (-1);
		}
		else if (!is_map_line(tab[i], "01NSEW ")
			|| find_plyr_pos(data, tab[i], i - map_start, &plyr_bool))
			return (-1);
	}
	return (0);
}
