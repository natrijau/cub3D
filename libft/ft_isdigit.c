/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_isdigit.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yanolive <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/10/31 18:53:20 by yanolive		  #+#	#+#			 */
/*   Updated: 2023/10/31 18:53:22 by yanolive		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}