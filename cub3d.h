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
#  define M_PI 3.1415926535897932384626433
# endif
# define BUFFER_SIZE 100
# define MOOVE_SPEED 1.5
# define ROTATE_SPEED (2 * M_PI / 90)
# define FOV 80
# define CASE 12
# define WIDTH 1300
# define HEIGHT 700
# define TRUE 1
# define FALSE 0
# define N (M_PI * 2)
# define E (M_PI * 1.5)
# define S (M_PI * 1)
# define W (M_PI * 0.5)

typedef struct	s_vec
{
	double	x;
	double	y;
}				t_vec;

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
	t_image	actual_wall;
	int		floor_color;
	int		ceiling_color;
	int		wall_color;
	double	distance;
	double	x;
	double	y;
}				t_raycast;


typedef struct s_hook
{
	int			keyboard_bool;
	int			move_forward;
	int			move_back;
	int			move_right;
	int			move_left;
	int			rotate_right;
	int			rotate_left;
}				t_hook;

typedef struct	s_data
{
	char		**map;
	void		*mlx;
	void		*win;
	t_raycast	raycast;
	t_hook		hook;
	double		angle;
	double		fov_rad;
	double		first_rayangle;
	double		angle_step;
	double		x;
	double		y;
	int			map_height;
	int			map_width;
}				t_data;

int			init_data(t_data *data, char **tab, int map_start);
void		ray_cast(t_data *data);
t_raycast	init_ray_cast(t_data *data);
int			init_cub3d(t_data *data);
void		creat_image(t_image *img, void *mlx, int width, int height);
t_image		get_wall(void *mlx, char *file);
void		ft_mlx_pixel_put(t_image *img, int x, int y, int color);
int			parsing(t_data *data, char *file);
void		print_map(char **map, int erase_bool);
int			map_len(char *file);
void		map_clear(char **map);
int			ft_strtablen(char **map);
int			get_max_tab_len(char **map);
char		*clear_space(char *str);
int			is_empty_line(const char *line);
int 		ft_isspace(char c);
// init_data_utils
int			check_color_value(char **tab);
t_image		get_wall(void *mlx, char *file);
int			add_direction_img(t_image *dest, t_image *src, char *str, char *direction);
double		get_angle(char direction);
char		*clear_space(char *src);

#endif