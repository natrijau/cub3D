#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include "libft/libft.h"
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif
# define BUFFER_SIZE 100
# define MOOVE_SPEED 2
# define ROTATE_SPEED (2 * M_PI / 90)
# define FOV 80
# define CASE 12
# define WIDTH 960
# define HEIGHT 540
# define TRUE 1
# define FALSE 0
# define N (M_PI * 2)
# define E (M_PI * 1.5)
# define S (M_PI * 1)
# define W (M_PI * 0.5)

typedef struct	s_image
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}				t_image;

typedef struct	s_ray
{
	char	**map;
	double	angle;
	double	x;
	double	y;
	double	x_step;
	double	y_step;
	int		x_multi;
	int		y_multi;
	char	flag;
}				t_ray;

typedef struct	s_raycast
{
	t_image	raycast;
	t_image	N_wall;
	t_image	E_wall;
	t_image	S_wall;
	t_image	W_wall;
	int		floor_color;
	int		ceiling_color;
	int		wall_color;
	double	distance;
	double	x;
	double	y;
}				t_raycast;

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

	char	**compass;
	char	**path_wall;	
	
	t_minimap	minimap;
	t_raycast	raycast;
	double		angle;
	double		fov_rad;
	double			x;
	double			y;
}				t_data;

char		**get_file(char *file);
void		ray_cast(t_data *data);
t_image		init_space(t_data *data);
t_raycast	init_ray_cast(t_data *data);
int			init_cub3d(t_data *data);
void		creat_image(t_image *img, void *mlx, int width, int height);
void		ft_mlx_pixel_put(t_image *img, int x, int y, int color);
int			parsing(t_data *data, char *file);
void		print_map(char **map, int erase_bool);
int			map_len(char *file);
int			init_start(char **map, t_data *data);
void		map_clear(char **map);
int			ft_strtablen(char **map);
char    	**tab_cpy(char **tab);

char		*clear_space(char *str);
int			is_map_line(char *line_map);
int			is_empty_line(const char *line);

#endif