/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_tab_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertet <gbertet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 14:12:52 by lamasson          #+#    #+#             */
/*   Updated: 2023/07/05 14:39:07 by lamasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_init_tab_env(char **env, t_mishell *mish)
{
	int		len;
	int		i;

	i = 0;
	len = ft_tablen(env);
	mish->files = malloc(sizeof(t_files) * 1);
	mish->files->tab_var_env = malloc(sizeof(char *) * (len + 1));
	while (env[i] != NULL)
	{
		mish->files->tab_var_env[i] = ft_strdup(env[i]);
		i++;
	}
	mish->files->tab_var_env[i] = NULL;
}

void	ft_free_tab_env(t_files *files)
{
	int	i;

	i = 0;
	while (files->tab_var_env[i] != NULL)
	{
		free(files->tab_var_env[i]);
		i++;
	}
	free(files->tab_var_env);
}

int	ft_tablen(char **tab)
{
	int	len;

	len = 0;
	while (tab[len] != NULL)
		len++;
	return (len);
}
