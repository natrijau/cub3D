/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:11 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/05 14:10:53 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_map_line(char *line_map, char *str)
{
	int	i;

	i = 0;
	while (line_map[i])
	{
		if (!ft_strchr(str, line_map[i]))
		{
			printf("Error\nInvalid map line\n");
			return (-1);
		}
		i++;
	}
	return (0);
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
	while (i >= 5)
	{
		j = 0;
		while (file_content[i][j] && ft_strchr("01NSEW ", file_content[i][j]))
			j++;
		if (file_content[i][j] && i == 5)
			return (i);
		else if (file_content[i][j])
			break ;
		i--;
	}
	return (-1);
}
