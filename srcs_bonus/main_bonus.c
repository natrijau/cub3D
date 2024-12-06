/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/04 17:25:10 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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
	if (data->raycast.n_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.n_wall.img);
	if (data->raycast.e_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.e_wall.img);
	if (data->raycast.s_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.s_wall.img);
	if (data->raycast.w_wall.img)
		mlx_destroy_image(data->mlx, data->raycast.w_wall.img);
	if (data->raycast.door.img)
		mlx_destroy_image(data->mlx, data->raycast.door.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	free_door_tab(data->tab_door);
	exit(EXIT_SUCCESS);
}

void	data_init(t_data *data)
{
	data->win = NULL;
	data->img_win.img = NULL;
	data->raycast.n_wall.img = NULL;
	data->raycast.e_wall.img = NULL;
	data->raycast.s_wall.img = NULL;
	data->raycast.w_wall.img = NULL;
	data->raycast.door.img = NULL;
	data->raycast.floor_color = 0x606060;
	data->raycast.ceiling_color = 0x3399ff;
	data->change_state_door = FALSE;
	data->door_frame = FALSE;
	data->map = NULL;
	data->tab_door = NULL;
	init_calculs(data);
}

// Move the character according to user input
void	shift(t_data *data, int y, int x)
{
	double	new_x;
	double	new_y;

	if ((!x && !y && data->angle == data->old_angle
			&& !data->change_state_door) && !data->door_frame)
		return ;
	data->door_frame = FALSE;
	data->old_angle = data->angle;
	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + data->calculs.west) * MOOVE_SPEED) * y;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_y += (sin(data->angle + data->calculs.west) * MOOVE_SPEED) * y;
	shift_collision(data, new_x, new_y);
	init_img_win(data);
	init_minimap(data);
	raycast(data);
	if (data->change_state_door == TRUE)
		data->change_state_door = FALSE;
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
}

int	check_type(char *str)
{
	int	i;

	i = ft_strlen(str) - 4;
	if (i < 0 || str[i] != '.' || str[i + 1] != 'c'
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
	mlx_mouse_hide(data.mlx, data.win);
	mlx_mouse_hook(data.win, handle_mouse, &data);
	mlx_hook(data.win, 17, 4, cub_close, &data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_loop_hook(data.mlx, update_move, &data);
	mlx_loop(data.mlx);
	cub_close(&data);
	return (0);
}
