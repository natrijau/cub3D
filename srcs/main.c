/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/11/29 16:17:57 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// close the window and free
int	cub_close(t_data *data)
{
	if (data->win)
	{
		mlx_clear_window(data->mlx, data->win);
		mlx_destroy_window(data->mlx, data->win);
	}
	if (data->img_win.img)
		mlx_destroy_image(data->mlx, data->img_win.img);
	if (data->raycast.N_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.N_wall.img);
	if (data->raycast.E_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.E_wall.img);
	if (data->raycast.S_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.S_wall.img);
	if (data->raycast.W_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.W_wall.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	exit(EXIT_SUCCESS);
}

void	data_init(t_data *data)
{
	data->win = NULL;
	data->img_win.img = NULL;
	data->raycast.N_wall.img = NULL;
	data->raycast.E_wall.img = NULL;
	data->raycast.S_wall.img = NULL;
	data->raycast.W_wall.img = NULL;
	data->raycast.floor_color = 0;
	data->raycast.ceiling_color = 0;
	data->map = NULL;
}

// Move the character according to user input
void	shift(t_data *data, int y, int x)
{
	double	new_x;
	double	new_y;

	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + W) * MOOVE_SPEED) * y;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_y += (sin(data->angle + W) * MOOVE_SPEED) * y;
	data->x += new_x;
	data->y += new_y;
	init_img_win(data);
	if (data->x > 2 && data->x < data->width - 2
		&& data->y > 2 && data->y < data->height - 2
		&& ft_strchr("01", data->map[(int)data->y / CASE][(int)data->x / CASE]))
		raycast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
}

int	check_type(char *str)
{
	int	i;

	i = ft_strlen(str) - 4;
	if (str[i] != '.' || str[i + 1] != 'c'
		|| str[i + 2] != 'u' || str[i + 3] != 'b')
		return (-1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2 || check_type(av[1]))
	{
		printf("Error\nInvalid arguments\n");
		return (1);
	}
	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data_init(&data);
	if (parsing(&data, av[1]) == -1 || init_cub3d(&data) == -1)
		cub_close(&data);
	mlx_hook(data.win, 17, 4, cub_close, &data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_loop_hook(data.mlx, update_move, &data);
	mlx_loop(data.mlx);
	cub_close(&data);
	return (0);
}
