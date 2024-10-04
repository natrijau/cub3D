#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"

# define BUFFER_SIZE 100
# define MOOVE_SPEED 4
# define ROTATE_SPEED 0.1
# define FOV 60
# define CASE 32
# define WIDTH 1920
# define HEIGHT 1080
# define TRUE 1
# define FALSE 0

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_image;

typedef struct	s_minimap
{
	int		height;
	int		width;
	t_image	space;
	t_image	character;
}				t_minimap;

typedef struct	s_data
{
	char		**map;
	void		*mlx;
	void		*win;
	t_minimap	minimap;
	double		angle;
	double		fov_rad;
	int			x;
	int			y;
}				t_data;

char	**get_map(char *file);
int		parsing(t_data *data, char *file);
void	print_map(char **map, int erase_bool);
int		map_len(char *file);
int		init_start(char **map, t_data *data);
void	map_clear(char **map);
int		ft_strtablen(char **map);

#endif