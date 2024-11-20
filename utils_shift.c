#include "cub3d.h"

// switch AZERTY or QWERTY
void	azerty_qwerty(int keycode, t_hook	*hook)
{
	if (keycode == 119 && hook->keyboard_bool == FALSE)
		hook->keyboard_bool = TRUE;
	else if (keycode == 122 && hook->keyboard_bool == TRUE)
		hook->keyboard_bool = FALSE;
	if ((keycode == 119 && hook->keyboard_bool == TRUE)
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
}

// keyboard key events
int	key_press(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	if (keycode == 65293)
		data->change_state_door = TRUE;
	azerty_qwerty(keycode, hook);
	if (keycode == 65307)
		cub_close(data);
	if (keycode == 65361)
		hook->rotate_right = TRUE;
	else if (keycode == 65363)
		hook->rotate_left = TRUE;
	return (0);
}

int	key_release(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	if (keycode == 65293)
		data->change_state_door = FALSE;
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
	if (keycode == 65361)
		hook->rotate_right = FALSE;
	if (keycode == 65363)
		hook->rotate_left = FALSE;
	return (0);
}

int	update_move(t_data *data)
{
	int	x;
	int	y;

	mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
	if (x != data->hook.old_x)
	{
		data->angle += (x - data->hook.old_x) * 0.001;
		if (x > WIDTH - 200)
			x = WIDTH - 200;
		else if (x < 200)
			x = 200;
		mlx_mouse_move(data->mlx, data->win, x, y);
		data->hook.old_x = x;
	}
	x = 0;
	y = 0;
	y -= data->hook.move_forward;
	y += data->hook.move_back;
	x += data->hook.move_right;
	x -= data->hook.move_left;
	data->angle -= ROTATE_SPEED * data->hook.rotate_right;
	data->angle += ROTATE_SPEED * data->hook.rotate_left;
	data->angle = fmod(data->angle, N);
	shift(data, y, x);
	return (0);
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
	if (data->map[(int)(data->y + new_y)
		/ CASE][(int)(data->x + new_x) / CASE] == '0'
		&& data->map[(int)(data->y + new_y)
		/ CASE][(int)(data->x) / CASE] == '0'
		&& data->map[(int)(data->y)
		/ CASE][(int)(data->x + new_x) / CASE] == '0')
	{
		data->x += new_x;
		data->y += new_y;
	}
	init_img_win(data);
	init_minimap(data);
	ray_cast(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img_win.img, 0, 0);
}
