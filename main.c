#include "cub3d.h"

void	print_map(char **map, int erase_bool)
{
    int char_count = 0;
    int line_count = 0;

    for (int i = 0; map[i]; i++) {
        char_count += printf("%s\n", map[i]);
        line_count++;
    }
    usleep(20000);
	if (!erase_bool)
		return ;
    for (int i = 0; i < line_count; i++) {
        printf("\033[A");
    }
    for (int i = 0; i < char_count; i++) {
        printf("\b \b");
    }
}

void	ft_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

t_image	init_space(t_data *data)
{
	t_image	img;
    int     x, y, i, j;

	img.img = mlx_new_image(data->mlx, data->minimap.width * CASE, data->minimap.width * CASE);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
    y = -1;
	while (++y < data->minimap.height)
    {
        x = -1;
        while (++x < data->minimap.width && data->map[y][x])
        {
            if (data->map[y][x] == '0')
            {
                i = -1;
                while (++i < CASE)
                {
                    j = -1;
                    while (++j < CASE)
                        ft_mlx_pixel_put(&img, x * CASE + i, y * CASE + j, 0x00FFFFFF);
                }
            }
        }
    }
    return (img);
}

t_image	init_character(void *mlx)
{
	t_image	img;
    int     i;
    int     j;

	img.img = mlx_new_image(mlx, CASE / 2, CASE / 2);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
    i = 0;
    while (i < CASE / 2)
    {
        j = 0;
        while (j < CASE / 2)
        {
            ft_mlx_pixel_put(&img, i, j, 0x00FF0000);
            ++j;
        }
        ++i;
    }
    return (img);
}

int    cub_close(t_data *data)
{
    mlx_clear_window(data->mlx, data->win);
    mlx_destroy_window(data->mlx, data->win);
    mlx_destroy_image(data->mlx, data->minimap.space.img);
    mlx_destroy_image(data->mlx, data->minimap.character.img);
    mlx_destroy_display(data->mlx);
    free(data->mlx);
    map_clear(data->map);
    exit(EXIT_SUCCESS);
}

void	moove(t_data *data, int x, int y)
{
    int new_x;
    int new_y;

    x *= 4;
    y *= 4;
    new_x = data->x + x;
    new_y = data->y + y;
    if (data->map[new_x / CASE][new_y / CASE] == ' '
        || data->map[(new_x + CASE - 1) / CASE][new_y / CASE] == ' '
        || data->map[new_x / CASE][(new_y + CASE - 1) / CASE] == ' '
        || data->map[(new_x + CASE - 1) / CASE][(new_y + CASE - 1) / CASE] == ' ')
        return ;
    data->x = new_x;
    data->y = new_y;
    mlx_clear_window(data->mlx, data->win);
    mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
    mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->y, data->x);
}

int	key_hook(int keycode, t_data *data)
{
    static int azerty;

    if (keycode == 114)
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
    // ESKAPE
    if (keycode == 65307)
        cub_close(data);

    if (azerty == FALSE)
    {
        // WASD
        if (keycode == 119)
	    	moove(data, -1, 0);
        if (keycode == 115)
	    	moove(data, 1, 0);
        if (keycode == 100)
	    	moove(data, 0, 1);
        if (keycode == 97)
	    	moove(data, 0, -1);
    }
    else
    {
        // ZQSD
        if (keycode == 122)
    		moove(data, -1, 0);
        if (keycode == 115)
    		moove(data, 1, 0);
        if (keycode == 100)
    		moove(data, 0, 1);
        if (keycode == 113)
    		moove(data, 0, -1);
    }
    // VISION
    if (keycode == 65363)
        printf("VISION RIGHT\n");
    if (keycode == 65361)
        printf("VISION LEFT\n");
    return (0);
}

int	create_minimap(t_data *data)
{
	data->minimap.height = ft_strtablen(data->map);
	data->minimap.width = ft_strlen(data->map[0]);
    data->minimap.space = init_space(data);
    data->minimap.character = init_character(data->mlx);
    mlx_put_image_to_window(data->mlx, data->win, data->minimap.space.img, 0, 0);
    mlx_put_image_to_window(data->mlx, data->win, data->minimap.character.img, data->y, data->x);
	return (0);
}

int    init_cub3d(t_data *data)
{
    int height;
    int width;

    height = ft_strtablen(data->map) * CASE;
    width = ft_strlen(data->map[0]) * CASE;

    data->mlx = mlx_init();
    if (!data->mlx)
        return (-1);
    data->win = mlx_new_window(data->mlx, width, height, "Cub3D");
    if (!data->win)
        return (-1);
    return (0);
}

int main(int ac, char **av)
{
	t_data	data;

    if (ac != 2)
    {
        printf("Error\n");
        return (1);
    }
	if (parsing(&data, av[1]) == -1)
		return (1);
	print_map(data.map, FALSE);
    if (init_cub3d(&data) == -1
        || create_minimap(&data) == -1)
		return (1);
	mlx_hook(data.win, 17, 4, cub_close, &data);
    mlx_hook(data.win, 2, 1L<<0, key_hook, &data);
	mlx_loop(data.mlx);
    cub_close(&data);
    return (0);
}
