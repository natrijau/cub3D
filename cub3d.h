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
# define WIDTH 1200
# define HEIGHT 700
# define MINIMAP_IMG_POS_X WIDTH - HEIGHT / 5
# define MINIMAP_IMG_POS_Y HEIGHT - HEIGHT / 5
# define PLAYER_IMG_POS_X WIDTH - HEIGHT / 10 - CASE / 2
# define PLAYER_IMG_POS_Y HEIGHT - HEIGHT / 10 - CASE / 2
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
	double	x_step_div;
	double	y_step_div;
	int		x_multi;
	int		y_multi;
	char	flag;
}				t_ray;

typedef struct	s_raycast
{
	t_image	N_wall;
	t_image	E_wall;
	t_image	S_wall;
	t_image	W_wall;
	t_image	D_wall;
	t_image	actual_wall;
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
}				t_minimap;

typedef struct s_hook
{
	int			keyboard_bool;
	int			move_forward;
	int			move_back;
	int			move_right;
	int			move_left;
	int			rotate_right;
	int			rotate_left;
	int			mouse_move;
	int			old_x;
}				t_hook;

typedef struct	s_data
{
	char		**map;
	void		*mlx;
	void		*win;
	t_image		img_win;
	t_minimap	minimap;
	t_raycast	raycast;
	t_hook		hook;
	int			**tab_door;
	int			door_closed;
	double		angle;
	double		fov_rad;
	double		first_rayangle;
	double		angle_step;
	double		x;
	double		y;
	int			height_and_case;
	int			width_and_case;
}				t_data;

int			init_data(t_data *data, char **tab, int map_start);
void		ray_cast(t_data *data);
void		init_minimap(t_data *data);
void		init_img_win(t_data *data);
int			init_cub3d(t_data *data);
void		creat_image(t_image *img, void *mlx, int width, int height);
t_image		get_wall(void *mlx, char *file);
void		ft_mlx_pixel_put(t_image *img, int x, int y, int color);
int			parsing(t_data *data, char *file);
void		print_map(char **map, int erase_bool);
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


//utils_map.c
int			map_len(char *file);
void		map_clear(char **map);
int			find_map_start(char **file_content);
int			flood_fil(char **map, char **space, int x, int i);
char		**init_map(char **map_off);


//utils_shift.c
void    azerty_qwerty(int keycode, t_hook	*hook);
int		key_press(int keycode, t_data *data);
int		key_release(int keycode, t_data *data);
int 	update_move(t_data *data);
void	shift(t_data *data, int y, int x);

//utils_bonus.c
int		find_door_in_map(char **str);
int		**malloc_door_tab(int	**tab_door, char **str, int i, int j);
int		**init_door_tab(char **str);

//main.c
void	replace_door(t_data *data);
int		cub_close(t_data *data);
#endif