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
# define MOOVE_SPEED 2
# define ROTATE_SPEED (2 * M_PI / 90)
# define FOV 80
# define CASE 12
# define WIDTH 1920
# define HEIGHT 960
# define TRUE 1
# define FALSE 0
# define N (M_PI * 2)
# define E (M_PI * 1.5)
# define S (M_PI * 1)
# define W (M_PI * 0.5)

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}				t_image;

typedef struct s_ray
{
	char	**map;
	double	angle;
	double	x;
	double	y;
	double	x_step;
	double	y_step;
}				t_ray;

typedef struct s_raycast
{
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

typedef struct s_data
{
	char		**map;
	void		*mlx;
	void		*win;
	t_image		img_win;
	t_raycast	raycast;
	t_hook		hook;
	double		height;
	double		width;
	double		angle;
	double		old_angle;
	double		fov_rad;
	double		first_rayangle;
	double		angle_step;
	double		x;
	double		y;
}				t_data;

//utils_map.c
int			is_map_line(char *line_map, char *str);
char		*clear_space(char *src);
int			map_len(char *file);
void		map_clear(char **map);
int			find_map_start(char **file_content);

//utils_shift.c
int			key_press(int keycode, t_data *data);
int			key_release(int keycode, t_data *data);

//utils.c
void		free_door_tab(int **tab);
int			ft_strtablen(char **map);
int			ft_isspace(char c);
int			is_empty_line(const char *line);
int			get_max_tab_len(char **map);

//parsing.c
char		**get_file(char *file);
int			check_texture(t_data *data);
int			init(t_data *data, char **file, int map_start);
int			parsing(t_data *data, char *path_file);

//raycast.c
int			ft_mlx_get_pixel_color(t_image *img, int x, int y);
void		set_texture_config(t_data *data, t_ray ray, t_raycast *raycast);
void		draw_wall(t_data *data, int x);
void		raycast_projection(t_data *data, t_ray *ray);
void		raycast(t_data *data);

//map.c
int			check_zero_inside(char **tab);
int			valid_zero_map(char **tab);
char		**init_map(char **map_off);

//init_cub3d.c
void		creat_image(t_image *img, void *mlx, int width, int height);
void		ft_mlx_pixel_put(t_image *img, int x, int y, int color);
void		init_img_win(t_data *data);
int			init_cub3d(t_data *data);

//init_data.c
int			init_color(t_data *data, char *str);
int			init_texture(t_data *data, char *str);
int			find_plyr_pos(t_data *data, char *line, int y, int *find_plyr);
int			init_data(t_data *data, char **tab, int map_start);

// init_data_utils.c
int			check_color_value(char **tab);
int			get_wall(t_image *dest, t_data *data, char *str, char *direction);
double		get_angle(t_data *data, char direction);

//main.c
int			cub_close(t_data *data);
void		data_init(t_data *data);
int			shift(t_data *data);
int			main(int ac, char **av);

#endif