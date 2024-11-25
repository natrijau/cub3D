#include "cub3d.h"

void    draw_minimap(t_data *data, double distance, int x, int y)
{
    if (distance <= CASE_DIV_PER_TWO)
        ft_mlx_pixel_put(&data->img_win,
            MINIMAP_IMG_POS_X + x, MINIMAP_IMG_POS_Y + y, 0x00FF0000);
    else if (distance <= HEIGHT_DIV_PER_TEN)
    {
        if (x + data->x - HEIGHT_DIV_PER_TEN >= 0
            && y + data->y - HEIGHT_DIV_PER_TEN >= 0
            && x + data->x - HEIGHT_DIV_PER_TEN < data->width_and_case
            && y + data->y - HEIGHT_DIV_PER_TEN < data->height_and_case
            && data->map[(int)((y + data->y - HEIGHT_DIV_PER_TEN) / CASE)]
            [(int)((x + data->x - HEIGHT_DIV_PER_TEN) / CASE)] == '0')
            ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + x,
                MINIMAP_IMG_POS_Y + y, 0x00A0A0A0);
        else
            ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + x,
                MINIMAP_IMG_POS_Y + y, 0x00000000);
    }
    else if (distance <= HEIGHT_DIV_PER_TEN + 1)
        ft_mlx_pixel_put(&data->img_win, MINIMAP_IMG_POS_X + x,
            MINIMAP_IMG_POS_Y + y, 0x00FFFFFF);    
}

// Fonction pour initialiser l'image représentant l'espace de la minimap
void	init_minimap(t_data *data)
{
	double	distance;
	int		x;
	int		y;

	y = 0;
	while (y < HEIGHT_DIV_PER_FIVE)
	{
		x = 0;
		while (x < HEIGHT_DIV_PER_FIVE)
		{
			distance = sqrt(pow(x - HEIGHT_DIV_PER_TEN, 2) + pow(y - HEIGHT_DIV_PER_TEN, 2));
            draw_minimap(data, distance, x, y);
			++x;
		}
		++y;
	}
}