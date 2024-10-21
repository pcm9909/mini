/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:24 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:25 by chunpark         ###   ########.fr       */
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
