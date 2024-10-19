/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:34:15 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/19 15:45:25 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>

extern int	g_status;

# define COMMAND 1
# define ARGUMENT 2
# define OPTION 3
# define PIPE 4
# define REDIRECTION_LEFT 5
# define REDIRECTION_RIGHT 6
# define APPEND 7
# define HEREDOC 8
# define UNKNOWN 9
# define EXECUTE 10

typedef struct s_token
{
	int		type;
	int		heredoc_index;
	char	*value;
}	t_token;

typedef struct s_redir
{
	int		type;
	char	*target;
}	t_redir;

typedef struct s_var
{
	char		*input;
	char		**cmd_list;
	char		**envp;
	char		**og_envp;
	int			is_redirect;
	int			input_redir;
	int			output_redir;
	int			commands;
	int			pipes;
	int			exit_code;
	int			nb_cmd;
	int			heredoc_count;
	int			*heredoc_fds;
}	t_var;

/*************** main ****************/

/************** signal ***************/
void	init_signal(void);
void	handle_signal(int sig);

/*************** parse ***************/
int		parse(t_var *variables);

/************* find_path *************/
char	*find_cmd_path(char *cmd);

/*************** pipes ***************/
void	handle_pipe(t_token **commands[], int num_commands, t_var *var);

/************ split_input ************/
char	**split_input(char const *s, char c);

/*************** utils ***************/
void	bubble_sort(char **arr, int n);
int		ft_strcmp(const char *s1, const char *s2);
int		is_builtins(char *cmd);

/********* check_characters **********/
void	check_characters(t_var *var, t_token **token_group);
char	*get_env_value(t_var *var, char *str, int i);

/************* commands ***************/
int		run_command(t_var *var, t_token **token_group);

/************* execute ****************/
int		execute_command(t_token **token_group, t_var *var);

/*********** split_tokens *************/
t_token	***split_tokens(t_var *var, t_token *tokens);

/************* free_shell *************/
void	free_list(char **list);
void	free_command(char ***commands);
void	free_token_groups(t_token ***token_groups);
void	free_env(char ***env);
void	free_shell(t_var *var, t_token *tokens, t_token ***token_groups);

/*************** tokens ***************/
void	free_tokens(t_token *tokens);
void	tokenize_cmd_list(t_var *var, t_token *tokens);
int		count_cmd_list(char **cmd_list);

/*********** redirections *************/
int		handle_redirect(t_var *var, t_token **tokens);

/************* builtins ***************/
int		handle_cd(t_token **token_group, t_var *var);
void	copy_env(t_var *var, char **envp);
int		handle_env(t_var *var);
int		handle_export(t_token **token_group, t_var *var);
int		unset(char *name, size_t name_len, t_var *var);
int		handle_unset(t_token **token, t_var *var);
int		print_env_sorted(t_var *var);
int		handle_heredoc(t_var *var, t_token *tokens);
void	close_heredoc_fds(t_var *var);
int		redirect_heredoc(t_var *var, t_token *token);
int		handle_pwd(t_var *var);
int		handle_echo(t_token **token_group);

int		my_exit(t_token **token);

/********** builtins_utils ************/
int		find_command_index(t_token **tokens, const char *command);
int		is_valid_identifier(const char *arg);

#endif
