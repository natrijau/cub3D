/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/11/16 16:37:57 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
// Display the map in console
void	print_map(char **map, int erase_bool)
{
	int	char_count;
	int	line_count;
	int	i;

	i = 0;
	char_count = 0;
	line_count = 0;
	while (map[i])
	{
		char_count += printf("%s\n", map[i]);
		line_count++;
		i++;
	}
	usleep(15000);
	if (!erase_bool)
		return ;
	i = 0;
	while (i++ < line_count)
		printf("\033[A");
	i = 0;
	while (i++ < char_count)
		printf("\b \b");
}

// close the window and free
int	cub_close(t_data *data)
{
	if (data->win)
	{
		mlx_mouse_show(data->mlx, data->win);
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
	if (data->raycast.D_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.D_wall.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	exit(EXIT_SUCCESS);
}

void	replace_door(t_data *data)
{
	int	i;

	i = 0;
	if (!data->tab_door)
		return ;
	while (data->tab_door[i])
	{
		if (data->door_closed == FALSE)
			data->map[data->tab_door[i][0]][data->tab_door[i][1]] = '0';
		else if (data->door_closed == TRUE)
			data->map[data->tab_door[i][0]][data->tab_door[i][1]] = 'D';
		i++;
	}
}

//! DECOMMENTE POUR JOUER AVEC MOI
// ___|___|_X_
// ___|_X_|___
//    |   | O 
// X = Nathan
// O = Yann

void	data_init_img(t_data *data)
{
	data->win = NULL;
	data->img_win.img = NULL;
	data->raycast.N_wall.img = NULL;
	data->raycast.E_wall.img = NULL;
	data->raycast.S_wall.img = NULL;
	data->raycast.W_wall.img = NULL;
	data->raycast.D_wall.img = mlx_xpm_file_to_image(
			data->mlx, "./textures/doortile.xpm",
			&data->raycast.D_wall.width,
			&data->raycast.D_wall.height);
	if (!data->raycast.D_wall.img)
		printf("Error\nFailed to load door texture: %s\n", "ures/doortile.xpm");
	data->raycast.D_wall.addr = mlx_get_data_addr(
			data->raycast.D_wall.img,
			&data->raycast.D_wall.bpp,
			&data->raycast.D_wall.line_len,
			&data->raycast.D_wall.endian);
	data->door_closed = TRUE;
	data->map = NULL;
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)
	{
		printf("Error\nInvalid number of arguments\n");
		return (1);
	}
	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	data_init_img(&data);
	if (parsing(&data, av[1]) == -1 || init_cub3d(&data) == -1)
		cub_close(&data);
	printf("Map:\n");
	print_map(data.map, FALSE);
	mlx_mouse_hide(data.mlx, data.win);
	mlx_hook(data.win, 17, 4, cub_close, &data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_loop_hook(data.mlx, update_move, &data);
	mlx_loop(data.mlx);
	cub_close(&data);
	return (0);
}
