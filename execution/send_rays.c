/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkhalil <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:41:45 by hkhalil           #+#    #+#             */
/*   Updated: 2022/10/31 03:46:45 by hkhalil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	get_inter_point(t_data *game, double *d, double ang)
{
	double	ray_ang;
	double	correcth;
	double	correctv;
	double	delta_xv = 0;
	double	delta_yv = 0;
	double	delta_xh = 0;
	double	delta_yh = 0;
	double	x_ver = 0;
	double	y_ver = 0;
	double	x_hor = 0;
	double	y_hor = 0;
	double	d_ver = 0;
	double	d_hor = 0;
	double	inter_x = 0;
	double	inter_y = 0;
	int	flag_ver = 0;
	int	flag_hor = 0;
	
	ray_ang =  norm_angle(ang);
	///hor_inter code
	//first intersection
	correcth = 0;
	y_hor = floor(game->player_y / game->cube) * game->cube;
	if (ray_ang > 0 && ray_ang < M_PI)
		y_hor += game->cube;
	else
		correcth = -1;
	x_hor = game->player_x + (y_hor - game->player_y) / tan(ray_ang);
	//finding delta x and delta y
	delta_yh = game->cube;
	if (!(ray_ang > 0 && ray_ang < M_PI))
		delta_yh *= -1;
	delta_xh = game->cube / tan(ray_ang);
	if ((!(ray_ang < M_PI_2  || ray_ang > 1.5*M_PI) && delta_xh > 0) || ((ray_ang < M_PI_2  || ray_ang > 1.5*M_PI) && delta_xh < 0))
		delta_xh *= -1;

	//ver_inter
	//first intersection
	correctv = 0;
	x_ver = floor(game->player_x / game->cube) * game->cube;
	if (ray_ang < M_PI_2  || ray_ang > 1.5*M_PI)
		x_ver += game->cube;
	else
		correctv = -1;
	y_ver = game->player_y + (x_ver - game->player_x)*tan(ray_ang);
	//finding delta y and delta x
	delta_xv = game->cube;
	if (!(ray_ang < M_PI_2  || ray_ang > 1.5*M_PI))
		delta_xv *= -1;
	delta_yv = game->cube * tan(ray_ang);
	if ((!(ray_ang > 0 && ray_ang < M_PI) && delta_yv > 0) || ((ray_ang > 0 && ray_ang < M_PI) && delta_yv < 0))
		delta_yv *= -1;
	while (1)
	{
		if (x_ver > 0 && y_ver > 0 && x_ver < game->window_width && y_ver < game->window_length)
		{
			if (wall(game, x_ver + correctv, y_ver, 0) == 1)
				break;
			x_ver += delta_xv;
			y_ver += delta_yv;
		}
		else
		{
			flag_ver = -1;
			break ;
		}
	}
	while (1)
	{
		if (x_hor > 0 && y_hor > 0 && x_hor < game->window_width && y_hor < game->window_length)
		{
			if (wall(game, x_hor, y_hor + correcth, 0) == 1)
				break;
			x_hor += delta_xh;
			y_hor += delta_yh;
		}
		else
		{
			flag_hor = -1;
			break ;
		}
	}
	if (flag_hor || flag_ver)
	{
		if (flag_ver)
		{
			inter_x = x_hor;
			inter_y = y_hor;
		}
		else
		{
			inter_x = x_ver;
			inter_y = y_ver;
		}
		
	}
	else
	{
		//compare d_ver and d_hor
		d_ver = hypot(game->player_x - x_ver, game->player_y - y_ver);
		d_hor = hypot(game->player_x - x_hor, game->player_y - y_hor);
		if (d_hor > d_ver)
		{
			inter_x = x_ver;
			inter_y = y_ver;
		}
		else
		{
			inter_x = x_hor;
			inter_y = y_hor;
		}
	}
	my_mlx_pixel_put(game, inter_x, inter_y, 0xFFFF00);
	DDA(game->player_x, game->player_y, inter_x, inter_y, game);
	*d = hypot(game->player_x - inter_x, game->player_y - inter_y);
}

void	send_rays(t_data *game)
{
	int	i;
	double	ray_angle;
	double	rays[game->num_of_rays];

	ray_angle = game->angle - game->fov/2;
	i = 0;
	while (i < game->num_of_rays)
	{
		get_inter_point(game, &rays[i], ray_angle);
		//debug
		dprintf(2, "d[%d] == %f\n", i, rays[i]);
		//end debug
		ray_angle += norm_angle(game->fov / game->num_of_rays);
		i++;
	}
}