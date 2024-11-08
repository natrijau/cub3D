#include "cub3d.h"

// Move the character according to user input
void	moove(t_data *data, int y, int x)
{
	double new_x;
	double new_y;

	// new positions based on angle and speed of movement
	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + W) * MOOVE_SPEED) * y;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_y += (sin(data->angle + W) * MOOVE_SPEED) * y;
	// printf("data->map[(int)(data->y + new_y) / CASE][(int)(data->x + new_x) / CASE] %c\n",data->map[(int)(data->y + new_y) / CASE][(int)(data->x + new_x) / CASE]);
	if (data->map[(int)(data->y + new_y) / CASE][(int)(data->x + new_x) / CASE] != '1'
		&& data->map[(int)(data->y + new_y) / CASE][(int)(data->x) / CASE] != '1'
		&& data->map[(int)(data->y) / CASE][(int)(data->x + new_x) / CASE] != '1')
	{
		data->x += new_x;  // Update x position
		data->y += new_y;  // Update y position
	}
	// Updating images after moving
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	mlx_destroy_image(data->mlx, data->raycast.raycast.img);
	data->minimap.space = init_space(data);  // Reinitialise map
	data->raycast = init_ray_cast(data);  // Reinitialise raycast
	ray_cast(data);  
	// New images to windows
	mlx_put_image_to_window(data->mlx, data->win, data->raycast.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - CASE / 2, data->y - CASE / 2);
}

// keyboard key events
int	key_press(int keycode, t_data *data)
{
	t_hook	*hook;

	hook = &data->hook;
	// switch AZERTY or QWERTY
	if (keycode == 119 && hook->keyboard_bool == FALSE)  // w
		hook->keyboard_bool = TRUE;
	else if (keycode == 122 && hook->keyboard_bool == TRUE)  // w
		hook->keyboard_bool = FALSE;
	// Escape key
	if (keycode == 65307)
		cub_close(data);
	// Déplacement mode QWERTY
	if ((keycode == 119 && hook->keyboard_bool == TRUE)
		|| (keycode == 122 && hook->keyboard_bool == FALSE))
		hook->move_forward = TRUE;
	else if (keycode == 97 && hook->keyboard_bool == TRUE)   // q
		hook->move_left = TRUE;
	else if (keycode == 113 && hook->keyboard_bool == FALSE)   // a
		hook->move_left = TRUE;
	else if (keycode == 115)  // s
		hook->move_back = TRUE;
	else if (keycode == 100)  // d
		hook->move_right = TRUE;
	// Rotate camera flèches Right/Left
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
	if ((keycode == 119 && hook->keyboard_bool == TRUE)
		|| (keycode == 122 && hook->keyboard_bool == FALSE))
		hook->move_forward = FALSE;
	else if (keycode == 97 && hook->keyboard_bool == TRUE)   // q
		hook->move_left = FALSE;
	else if (keycode == 113 && hook->keyboard_bool == FALSE)   // a
		hook->move_left = FALSE;
	else if (keycode == 115)  // s
		hook->move_back = FALSE;
	else if (keycode == 100)  // d
		hook->move_right = FALSE;
	// Rotate camera flèches Right/Left
	if (keycode == 65361)
		hook->rotate_right = FALSE;
	if (keycode == 65363)
		hook->rotate_left = FALSE;
	return (0);
}

int update_move(t_data *data)
{
	int 	x;
	int 	y;

	// rotate avec la souris
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
    // Vérifier les booléens
	x = 0;
	y = 0;
    y -= data->hook.move_forward;
    y += data->hook.move_back;
    x += data->hook.move_right;
	x -= data->hook.move_left;
	data->angle -= ROTATE_SPEED * data->hook.rotate_right;  // Left
	data->angle += ROTATE_SPEED * data->hook.rotate_left;  // Right
	data->angle = fmod(data->angle, N);  // Limit angle  ( 0 ; N )
	moove(data, y, x);
    return (0);
}