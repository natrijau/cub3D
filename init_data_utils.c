#include "cub3d.h"

int	check_color_value(char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j] && i < 3)
		{
			if (!ft_isdigit(tab[i][j]) || j > 2)
				return (-1);
			j++;
		}
		if (ft_atoi(tab[i]) > 255 || i > 2)
			return (-1);
		i++;
	}
	return (0);
}

t_image	get_wall(void *mlx, char *file)
{
	t_image	img;

	img.img = mlx_xpm_file_to_image(mlx, file, &img.width, &img.height);
	if (img.img)
		img.addr = mlx_get_data_addr(img.img,
				&img.bpp, &img.line_len, &img.endian);
	else
		printf("Error\nInvalid texture path: %s\n", file);
	return (img);
}

int	add_direction_img(t_image *dest, t_image *src, char *str, char *direction)
{
	if (!ft_strncmp(str, direction, 2))
	{
		if (!dest)
		{
			printf("Error\nDouble texture definition: %s\n", direction);
			return (-1);
		}
		*dest = *src;
	}
	return (0);
}

// returns the starting angle based on the direction
double	get_angle(char direction)
{
	if (direction == 'N')
		return (N);
	if (direction == 'E')
		return (W);
	if (direction == 'S')
		return (S);
	if (direction == 'W')
		return (E);
	printf("Error\nInvalid player direction: %c\n", direction);
	return (-1);
}

char	*clear_space(char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (!ft_isspace(src[i]))
		{
			src[j] = src[i];
			j++;
		}
		i++;
	}
	src[j] = '\0';
	return (src);
}
