/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbertet <gbertet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:30:57 by lamasson          #+#    #+#             */
/*   Updated: 2023/07/05 15:55:48 by lamasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h> //fork, wait, waitpid, wait3, wait4, open
# include <sys/wait.h> //wait
# include <fcntl.h> //access, open, unlink
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free, getenv, exit
# include <unistd.h> //write, read, pipe, dup, dup2, execve, getcwd, chdir
# include <readline/readline.h> //readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <readline/history.h>
# include <signal.h> //signal, sigaction, sigemptyset, sigaddset, kill
# include <errno.h> //strerror, perror
# include <sys/stat.h> //stat, lstat, fstat
# include <dirent.h> //opendir, readdir, closedir 
# include <sys/time.h> //?
# include <termios.h> //tcsetattr, tcgetattr, isatty, ttyname, ttyslot, ioctl
# include "../libft/libft.h"

extern int	g_status;

typedef struct s_mishell
{
	struct s_cmd	*cmds;		//tableau de structure des commandes
	char			**av;		//tab argv du main
	char			*full_cmd;	//ligne de cmd du terminal
	char			path[1024];
	int				nb_cmds;	//nombre de commande dans la ligne
	int				pos_cmd;	//pos dans tab de cmds pdt execution des pipes
	int				type;
	int				*here_doc;
	char			**env;		//variable d'environnements du main 
	struct s_files	*files;
	pid_t			*pid;
}               t_mishell;

typedef struct	s_cmd			//tableau de struct
{
	struct s_fds	*fds;
	char			**c;				//tableau cmd avc opt et arg pour chaque
	char			*path;				//path correspondant a la cmd ou NULL si builtin 
	int				here_doc;
}				t_cmd;

typedef struct 	s_fds
{
	char	*fd_in;
	char	*fd_out;
	int		in;
	int		out; // -1 = no fichier out / 0 redirection simple / 1 redirection append mode
	int		err;
}				t_fds;

typedef struct s_files{
	char	**tab_var_env;	//notre tableau de variables d'environnements
	char	**tab_path;		//PATH val split, tableau de tous les paths ':'
}t_files;

typedef struct s_var_env{
	int					nb_dol;
	char				*str;
	int					len;
	int					pos;
	char				*tmp;
	struct s_new_str	*val;
}t_var_env;

typedef struct s_new_str{
	int		start;
	char	*name;
	int		len_n;
	char	*val;
	int		len_v;
}t_new_str;

//		MAIN.C					//
void	get_cmds(t_mishell *m);

//		FT_ENV.C				//
int		ft_env(t_files files, char **c);

//		FT_EXPORT.C				//
int		ft_export(char **c, t_files *files);
void	switch_env(t_files *files, char *name, char *str);
void	ft_realloc_tab_env(t_files *files, char *str);
char	*rec_var_env(char *str);
int		ft_parse_name(char *str);

//		FT_EXPORT_UTILS.C		//
int		ft_export_no_arg(t_files files);
void	ft_free_n_tab(char **tab, int n);
char	*concat_export(char *env_var, char *str);
int		env_var_found(char **tab, char *name, char *c);

//		FT_EXPORT_PARSING.C		//
int		ft_parse_len(char **c, t_files *files);
int		ft_parse_name_export(char **c, int j);
void	ft_error_export(char *c, int g, char *cmd);
int		check_egal(char *c);

//		FT_UNSET.C				//
int		ft_unset(char **c, t_files *files);

//		FT_PWD.C				//
int		ft_pwd(char **c);

//		FT_CD.C					//
int		ft_cd(char **c, t_files *files);
int		parsing_cd(char **c);
int		ft_user_path(char **c);
char	*ft_home_directory(void);

//		FT_ECHO.C				//
void	ft_echo_arg(char *s);
int		ft_echo(char **cmd);


//		FT_CHECK_BUILTINS.C		//
int		check_built_no_fork(char **c, t_files *files, t_mishell *m); //exec built
int		check_built_fork(char **c, t_files *files); //exec built
int		check_if_cmd_built(t_cmd cmds); //return (0) si no built ou (1) si built

//		FT_UTILS.C				//
int		ft_iswhitespace(char c);
int		ft_betweenchar(char *s, int pos, char cquote);
int		ft_betweenquotes(char *s, int pos);
int		ft_strstrlen(char **s);
char	**ft_strstrdup(char **s);

//		FT_READLINE.C			//
char	*ft_readline(char *str);
int		ft_path_pos_prompt(void);
char	*ft_read_here_doc(char *prompt, char *eof);

//		FT_NORMALIZE_LINE.C		//
char	*normalize_str(char *s, t_files *files);
char	**ft_remove_redirections(char **cmd);
char	*ft_remove_quotes(char *s);
char	*remove_char(char *s, int pos);
char	*ft_add_char(char *s, char c, int pos);

//		PARSING_REDIRECTION.C	//
t_fds	*parsing_fd(char **str);

//		PARSING_RIGHT_FILE.C	//
int		ft_check_fd(char **tmp_fds, int *type);
void	set_fd(char **tmp_fds, t_fds *fds, int *type);

//		FT_EXIT.C				//
void	ft_free_str(char **s);
void	ft_free_cmds(t_mishell *m);
void	ft_free_files(t_mishell *mish);
int		ft_exit(t_mishell *m);
void	ft_free_n_tab(char **tab, int n);

//		FT_SPLIT_MINISHELL.C	//
char	**ft_split_minishell(char const *s, char c);

//		FT_PARSING_CMD.C		//
int		empty_str(const char *s);
int		synthax_check(char *s);
int		check_pipe(char *s);

//		FT_INIT_TAB_ENV.C		//
void	ft_init_tab_env(char **env, t_mishell *mish);
void	ft_free_tab_env(t_files *files);
int		ft_tablen(char **tab);

//		FT_MAJ_TAB_ENV.C		//HOME, USER, _ :pas fait  
int		maj_tab_env_oldpwd(t_files *files); //appel en 1er recup pwd pour ca maj
int		maj_tab_env_pwd(t_files *files);

//		FT_GET_PATH_CMD.C		//
char	**ft_get_tab_path(t_files files);
void	ft_init_path_cmd(t_mishell *mish, t_files files, int j);
void	ft_cmd_path_ready(t_mishell *mish);

//		FT_PIPEX.C				//
int		ft_call_pipex(t_mishell *mish); //appel pipe -> fork -> dup et exec_cmd

//		FT_PIPEX_UTILS.C		//
void	ft_check_status_exec(t_mishell *m);
int		open_fdout(t_fds fds);
int		open_fdin(t_mishell *m, int fd_in);
int		ft_check_pipe_and_exit(t_mishell *m);
int		ft_init_tab_pid(t_mishell *m);

//		FT_STRJOIN_PATH.C		//
char	*ft_strjoin_path(char *path, char *cmd);

//		FT_EXEC.C				//
int		ft_exec_cmd(t_mishell *mish);

//		FT_PARSING_VAR_ENV.C	//
char	*ft_handle_var_env(char *str, t_files files);

//		FT_PARSING_VAR_ENV_UTILS.C	//
int		ft_check_dollar(char *str, int i);
int		ft_check_end_name(char *str, int i);
void	ft_str_data(char *str, t_var_env *data);
void	ft_free_data_var_env(t_var_env *data, int n);
char	*rec_name_var_env(char *str);

//		FT_JOIN_VAR_ENV_STR.C	//
void	ft_join_all_str(t_var_env *data);
char	*ft_strjoin_loop(char *tmp, char *buf);
void	ft_val_g_status(t_var_env *data, int j);

//		SIGNAL.C				//
void	sigint_fork(int signum);
void	sigquit_fork(int signum);
void	sigint_outfork(int signum);
void	signal_maj_outfork(void);
int		signal_check_readline(void);

//		FT_HEREDOC.C			//
void    ft_heredoc(char **cmds);

//		FT_FREE.C				//
void	ft_free_n_tab(char **tab, int n);
void	ft_free_str(char **s);
void	ft_free_cmds(t_mishell *m);
void	ft_free_files(t_mishell *mish);

#endif
