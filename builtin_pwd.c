/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:24 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 21:14:10 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	handle_pwd_command(void)
{
	char	*tmp_pwd;

	tmp_pwd = getcwd(NULL, BUFSIZ);
	printf("%s\n", tmp_pwd);
	free(tmp_pwd);
}
