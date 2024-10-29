/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/10/28 15:33:26 by yanolive         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Display the map in console
void	print_map(char **map, int erase_bool)
{
	int	char_count = 0;
	int	line_count = 0;

	for (int i = 0; map[i]; i++) {
		char_count += printf("%s\n", map[i]);
		line_count++;
	}
	usleep(15000);
	if (!erase_bool)
		return ;
	for (int i = 0; i < line_count; i++) {
		printf("\033[A");
	}
	for (int i = 0; i < char_count; i++) {
		printf("\b \b");
	}
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
	if (data->minimap.space.img)
		mlx_destroy_image(data->mlx, data->minimap.space.img);
	if (data->minimap.character.img)
		mlx_destroy_image(data->mlx, data->minimap.character.img);
	if (data->raycast.raycast.img)
		mlx_destroy_image(data->mlx, data->raycast.raycast.img);
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

#include <unistd.h>

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

//! DECOMMENTE POUR JOUER AVEC MOI
// ___|___|___
// ___|_X_|___
//    |   | O 
// X = Nathan
// O = Yann

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

void	data_init_img(t_data *data)
{
	data->win = NULL;
	data->minimap.space.img = NULL;
	data->minimap.character.img = NULL;
	data->raycast.raycast.img = NULL;
	data->raycast.N_wall.img = NULL;
	data->raycast.E_wall.img = NULL;
	data->raycast.S_wall.img = NULL;
	data->raycast.W_wall.img = NULL;
	data->map = NULL;
}

int main(int ac, char **av)
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
	mlx_hook(data.win, 17, 4, cub_close, &data);  // Defin hook close windows
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);  // Defin hook keys
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);  // bouton relachee
	mlx_loop_hook(data.mlx, update_move, &data);
	mlx_loop(data.mlx);  // Principal loop
	cub_close(&data);
	return (0);
}

//TODO DDA ?
//TODO MINIMAP RONDE
//TODO PORTES ?????? pas sur
//TODO NORMER DES FONCTIONS
//TODO TRIER LES FICHIERS