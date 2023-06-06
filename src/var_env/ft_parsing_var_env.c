/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_var_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamasson <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:00:23 by lamasson          #+#    #+#             */
/*   Updated: 2023/06/06 17:23:35 by lamasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_wheel_tab_var_env(t_files files, t_var_env *data, int j)
{
	int		i;
	int		len_tmp;
	char	*tmp;

	i = 0;
	while (files.tab_var_env[i] != NULL)
	{
		tmp = rec_name_var_env(files.tab_var_env[i]);
		len_tmp = ft_strlen(tmp);
		if (ft_strncmp(tmp, data->val[j].name, len_tmp) == 0 && \
			len_tmp == data->val[j].len_n)
		{
			free(tmp);
			data->val[j].len_v = ft_strlen(files.tab_var_env[i]);
			data->val[j].val = ft_substr(files.tab_var_env[i], len_tmp + 1, \
				data->val[j].len_v - (len_tmp + 1));
			return (0);
		}
		free(tmp);
		i++;
	}
	data->val[j].len_v = 0;
	data->val[j].val = "\0";
	return (-1);
}

static void	ft_init_new_str(t_files files, t_var_env *data)
{
	int		i;

	i = 0;
	while (i < data->nb_dol)
	{
		ft_wheel_tab_var_env(files, data, i);
		i++;
	}
	data->val[i].val = NULL;
	ft_join_all_str(data);
}

static void	ft_parse_struct_var_env(char *str, t_var_env *data)
{
	int	i;

	i = 0;
	while (str[data->len] != '\0')
	{
		if (str[data->len] == '$')
			data->nb_dol++;
		data->len++;
	}
	data->val = malloc(sizeof(t_new_str) * (data->nb_dol + 1));
	data->val[data->nb_dol].name = NULL;
	while (i < data->nb_dol)
	{
		data->val[i].start = ft_check_dollar(str, data->pos);
		data->pos = data->val[i].start;
		data->val[i].len_n = ft_check_end_name(str, data->pos);
		data->val[i].len_n = data->val[i].len_n - data->val[i].start;
		data->val[i].name = ft_substr(str, data->val[i].start, \
			data->val[i].len_n);
		if (!data->val[i].name)
			exit (1);
		i++;
	}
}

char	*ft_handle_var_env(char *str, t_files files)
{
	t_var_env	data;
	char		*new_str;

	data.str = str;
	data.len = 0;
	data.nb_dol = 0;
	data.pos = 0;
	if (ft_check_dollar(str, 0) == -1)
		return (str);
	ft_parse_struct_var_env(str, &data);
	ft_init_new_str(files, &data);
	new_str = ft_strdup(data.tmp);
	ft_free_data_var_env(&data, data.nb_dol);
	return (new_str);
}
/*
int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	char	*str;
	char	*new;

	str = "lol$USER$USER$USERSS.LDL.shh"; // = lamasson.SDSS $USER$USER = lamassonlamasson $USERldl = ""
	t_files	files;
	ft_init_tab_env(env, &files);

	new = ft_handle_var_env(str, files);
	
	printf("%s\n", new);

	ft_free_tab(files.tab_var_env);
	free(new);
	return (0);
}*/