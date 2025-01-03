/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:57:24 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 14:51:20 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	is_map_line(char *line_map, char *str)
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
				break ;
			}
			j++;
		}
		if (str[j] == '\0' && printf("Error\nInvalid map line\n"))
			return (0);
		i++;
	}
	return (1);
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

int	find_map_start(char **file_content)
{
	int	i;
	int	j;

	i = ft_strtablen(file_content) - 1;
	if (i < 6)
		return (-1);
	while (i >= 3)
	{
		j = 0;
		while (file_content[i][j] && ft_strchr("01NSEWD ", file_content[i][j]))
			j++;
		if (file_content[i][j] && i <= 6)
			return (i);
		else if (file_content[i][j])
			break ;
		i--;
	}
	return (-1);
}
