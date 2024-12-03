/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_shift.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanolive <yanolive@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:00:14 by yanolive          #+#    #+#             */
/*   Updated: 2024/12/03 13:00:15 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// keyboard key events
int	key_press(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	if (keycode == 119 && hook->keyboard_bool == FALSE)
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
	else if (keycode == 65307)
		cub_close(data);
	else if (keycode == 65361)
		hook->rotate_right = TRUE;
	else if (keycode == 65363)
		hook->rotate_left = TRUE;
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
