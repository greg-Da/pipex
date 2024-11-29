/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: greg <greg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:33:47 by greg              #+#    #+#             */
/*   Updated: 2024/11/29 14:47:50 by greg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    *ft_realloc(void *ptr, size_t size)
{
	void    *new;

	if (size == 0) {
		free(ptr);
		return NULL;
	}
	if (ptr == NULL) {
		return malloc(size);
	}
	new = malloc(size);
	if (new == NULL)
		return NULL;
	ft_memcpy(new, ptr, size);
	free(ptr);
	return (new);
}
