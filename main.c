/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:07:41 by yanolive          #+#    #+#             */
/*   Updated: 2024/10/23 18:10:37 by natrijau         ###   ########.fr       */
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
		printf("\033[A");  // Remonter le curseur dans la console d'un nombre de lignes égal à line_count
	}
	for (int i = 0; i < char_count; i++) {
		printf("\b \b");  // Effacer les caractères en réinitialisant leur espace
	}
}

// close the window and free
int	cub_close(t_data *data)
{
	mlx_clear_window(data->mlx, data->win);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	mlx_destroy_image(data->mlx, data->minimap.character.img);
	mlx_destroy_image(data->mlx, data->raycast.raycast.img);
	mlx_destroy_image(data->mlx, data->raycast.N_wall.img);
	mlx_destroy_image(data->mlx, data->raycast.E_wall.img);
	mlx_destroy_image(data->mlx, data->raycast.S_wall.img);
	mlx_destroy_image(data->mlx, data->raycast.W_wall.img);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	map_clear(data->map);
	exit(EXIT_SUCCESS);
}

// Move the character according to user input
void	moove(t_data *data, int y, int x)
{
	double new_x;
	double new_y;

	// new positions based on angle and speed of movement
	new_x = (cos(data->angle) * MOOVE_SPEED) * x;
	new_y = (sin(data->angle) * MOOVE_SPEED) * x;
	new_x += (cos(data->angle + W) * MOOVE_SPEED) * y;
	new_y += (sin(data->angle + W) * MOOVE_SPEED) * y;
	if (data->map[(int)(data->y + new_y) / CASE][(int)(data->x + new_x) / CASE] == '1')
		return ;
	data->x += new_x;  // Update x position
	data->y += new_y;  // Update y position

	// Updating images after moving
	mlx_destroy_image(data->mlx, data->minimap.space.img);
	mlx_destroy_image(data->mlx, data->raycast.raycast.img);
	data->minimap.space = init_space(data);  // Reinitialise map
	creat_image(&data->raycast.raycast, data->mlx, WIDTH, HEIGHT);  // Reinitialise raycasting
	ray_cast(data);  
	// New images to windows
	mlx_put_image_to_window(data->mlx, data->win, data->raycast.raycast.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
	mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->x - CASE / 2, data->y - CASE / 2);
}

// keyboard key events
int	key_hook(int keycode, t_data *data)
{
	static int azerty;

	
	// switch AZERTY or QWERTY
	if (keycode == 114)  // keyboard r
	{
		if (azerty == TRUE)
		{
			azerty = FALSE;
			printf("Keyboard config set to QWERTY\n");
		}
		else
		{
			azerty = TRUE;
			printf("Keyboard config set to AZERTY\n");
		}
	}
	// Escape key
	if (keycode == 65307)
		cub_close(data);

	if (azerty == FALSE)
	{
		// Déplacement mode QWERTY
		if (keycode == 119)  // w
			moove(data, -1, 0);
		if (keycode == 115)  // s
			moove(data, 1, 0);
		if (keycode == 100)  // d
			moove(data, 0, 1);
		if (keycode == 97)   // a
			moove(data, 0, -1);
	}
	else
	{
		// Déplacement mode AZERTY
		if (keycode == 122)  // z
			moove(data, -1, 0);
		if (keycode == 115)  // s
			moove(data, 1, 0);
		if (keycode == 100)  // d
			moove(data, 0, 1);
		if (keycode == 113)  // q
			moove(data, 0, -1);
	}
	// Rotate camera flèches Right/Left
	if (keycode == 65361)
		data->angle -= ROTATE_SPEED;  // Left
	if (keycode == 65363)
		data->angle += ROTATE_SPEED;  // Right
	data->angle = fmod(data->angle, N);  // Limit angle  ( 0 ; N )
	if (keycode == 65361 || keycode == 65363)
		moove(data, 0, 0);  // Recalcul position after rotate
	return (0);
}
// Fonction appelée lorsqu'il y a un mouvement de souris
int mouse_move(int x, int y, t_data *data)
{
	(void) y;
	int	center_x;
	center_x = WIDTH / 2;
    if (abs(x - center_x) < 30) // abs renvoie la valeur absolue de la diff entre position actuelle souris (x) et position centrale de l'écran (center_x)
		return (0);	// si mouvement inferieur a 7 pixel on ignore (evite pleins de recalcul inutiles)
	if (x > center_x)
		data->angle += (ROTATE_SPEED / 1.5);  // rotate Right
	else if (x < center_x)
		data->angle -= (ROTATE_SPEED / 1.5);  // rotate Left
	data->angle = fmod(data->angle, N);
	mlx_mouse_move(data->mlx, data->win, center_x, HEIGHT / 2);
	moove(data, 0, 0);
    return (0);
}

//! DECOMMENTE POUR JOUER AVEC MOI
// __|___|__
// __|_X_|__
//   |   |
// X = Nathan
// O = Yan

int main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)  // Verifi arg
	{
		printf("Error\n");
		return (1);
	}
	data.mlx = mlx_init();
	if (!data.mlx)
		return (1);
	if (parsing(&data, av[1]) == -1)  // Parsing map
	{
		//! juste pour tester
		// mlx_destroy_image(data.mlx, data.minimap.space.img);
		// mlx_destroy_image(data.mlx, data.minimap.character.img);
		// mlx_destroy_image(data.mlx, data.raycast.raycast.img);
		// mlx_destroy_image(data.mlx, data.raycast.N_wall.img);
		// mlx_destroy_image(data.mlx, data.raycast.E_wall.img);
		// mlx_destroy_image(data.mlx, data.raycast.S_wall.img);
		// mlx_destroy_image(data.mlx, data.raycast.W_wall.img);
		// mlx_destroy_display(data.mlx);
		free(data.mlx);
		map_clear(data.map);
		return (1);
	}
	print_map(data.map, FALSE); 
	if (init_cub3d(&data) == -1)
		return (1);
	mlx_mouse_hide(data.mlx, data.win);
	mlx_hook(data.win, 17, 4, cub_close, &data);  // Defin hook close windows
	mlx_hook(data.win, 2, 1L<<0, key_hook, &data);  // Defin hook keys
	mlx_hook(data.win, 6, 1L<<6 , mouse_move, &data);  // Defin hook moove mouse
	mlx_loop(data.mlx);  // Principal loop
	cub_close(&data);
	return (0);
}

//? A tester : limite de 131470 Char pour la map

//TODO voir parsing 
//TODO modif RAYCASTING POUR AFFICHER LES MURS COORESPONDANT A LA DIRECTION
//TODO 
//TODO stack over