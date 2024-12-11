/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_shift_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:57:30 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/09 12:42:45 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	handle_mouse(int button, int x, int y, t_data *data)
{
	(void) x;
	(void) y;
	if (button == 1)
		data->change_state_door = TRUE;
	return (0);
}

// keyboard key events
int	key_press(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	if (keycode == 65363)
		hook->rotate_left = TRUE;
	else if (keycode == 65361)
		hook->rotate_right = TRUE;
	else if (keycode == 65307)
		cub_close(data);
	else if (keycode == 119 && hook->keyboard_bool == FALSE)
		hook->keyboard_bool = TRUE;
	else if (keycode == 122 && hook->keyboard_bool == TRUE)
		hook->keyboard_bool = FALSE;
	else if ((keycode == 119 && hook->keyboard_bool == TRUE)
		|| (keycode == 122 && hook->keyboard_bool == FALSE))
		hook->move_forward = TRUE;
	else if (keycode == 97 && hook->keyboard_bool == TRUE)
		hook->move_left = TRUE;
	else if (keycode == 113 && hook->keyboard_bool == FALSE)
		hook->move_left = TRUE;
	else if (keycode == 115)
		hook->move_back = TRUE;
	else if (keycode == 100)
		hook->move_right = TRUE;
	return (0);
}

int	key_release(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	if ((keycode == 119 && hook->keyboard_bool == TRUE)
		|| (keycode == 122 && hook->keyboard_bool == FALSE))
		hook->move_forward = FALSE;
	else if (keycode == 97 && hook->keyboard_bool == TRUE)
		hook->move_left = FALSE;
	else if (keycode == 113 && hook->keyboard_bool == FALSE)
		hook->move_left = FALSE;
	else if (keycode == 115)
		hook->move_back = FALSE;
	else if (keycode == 100)
		hook->move_right = FALSE;
	else if (keycode == 65361)
		hook->rotate_right = FALSE;
	else if (keycode == 65363)
		hook->rotate_left = FALSE;
	return (0);
}

int	update_move(t_data *data)
{
	int	x;
	int	y;
	show_animation(data);
	mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
	if (x != data->hook.old_x)
	{
		data->angle += (x - data->hook.old_x) * 0.001;
		if (x >= WIDTH - 2 || x <= 2)
		{
			if (x >= WIDTH - 2)
				x = x - WIDTH + 2;
			else if (x <= 2)
				x = WIDTH - x - 2;
			mlx_mouse_move(data->mlx, data->win, x, y);
		}
		data->hook.old_x = x;
	}
	x = data->hook.move_right - data->hook.move_left;
	y = data->hook.move_back - data->hook.move_forward;
	data->angle -= data->calculs.rotate_speed * data->hook.rotate_right;
	data->angle += data->calculs.rotate_speed * data->hook.rotate_left;
	data->angle = fmod(data->angle, data->calculs.north);
	shift(data, y, x);
	return (0);
}

void	shift_collision(t_data *data, double x, double y)
{
	if (data->map[(int)(data->y)
		/ CASE][(int)(data->x + x) / CASE] != '0')
	{
		y *= 1.5;
		x = 0;
	}
	if (data->map[(int)(data->y + y)
		/ CASE][(int)(data->x) / CASE] != '0')
	{
		x *= 1.5;
		y = 0;
	}
	if (data->map[(int)(data->y + y)
		/ CASE][(int)(data->x + x) / CASE] == '0')
	{
		data->x += x;
		data->y += y;
	}
}
