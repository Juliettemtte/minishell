/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:34:15 by jmouette          #+#    #+#             */
/*   Updated: 2024/10/03 17:26:34 by jmouette         ###   ########.fr       */
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

extern char	**environ;

# define COMMAND 1
# define ARGUMENT 2
# define OPTION 3
# define PIPE 4
# define REDIRECTION_LEFT 5
# define REDIRECTION_RIGHT 6
# define APPEND 7
# define HEREDOC 8
# define UNKNOWN 9
/*
typedef enum e_token_type
{
	COMMAND,
	ARGUMENT,
	OPTION,
	PIPE,
	REDIRECTION_LEFT,
	REDIRECTION_RIGHT,
	APPEND,
	HEREDOC,
	UNKNOWN
}	t_token_type;
*/
typedef struct s_token
{
	int		type;
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
	char		*heredoc;
	int			is_redirect;
	int			commands;
	int			pipes;
	int			status;
	int			nb_cmd;
}	t_var;

/*************** main ****************/

/*************** check ***************/
int		check_quotes(char *input);
int		check_format(char *arg);

/************** signal ***************/
void	init_signal(void);

/*************** parse ***************/
int		parse(t_var *variables);

/************* find_path *************/
char	*find_cmd_path(char *cmd);

/*************** pipes ***************/
void	handle_pipe(t_token **commands[], int num_commands, t_var *var);
void	execute_pipes(t_var *var, t_token ***token_groups);

/************ split_input ************/
char	**split_input(char const *s, char c);

/*************** utils ***************/
void	bubble_sort(char **arr, int n);
int		ft_strcmp(const char *s1, const char *s2);
int		is_builtins(char *cmd);

/**************redirect***************/
int		redirectreplace_output_right(char *target);
int		redirect_output_right(char *target);
int		check_redirect(char **cmd_list);

/************* builtins **************/
int		handle_cd(t_token **token_group);
void	handle_env(void);
void	handle_export(char *name, char *value);
int		handle_unset(char *name, size_t name_len);
void	print_env_sorted(void);
int		my_exit(char **arg);

/************ builtins2 **************/
int		handle_heredoc(t_var *var);
int		handle_pwd(t_var *var);
int		handle_echo(t_token **token_group);

/************* commands ***************/
int		run_command(char *cmd, t_var *var, t_token **token_group);

/*************split_tokens*************/
t_token	***split_tokens(t_var *var, t_token *tokens);

/************* free_shell *************/
void	free_list(char **list);
void	free_command(char ***commands, int nb_cmd);

/*************** tokens ***************/
void	tokenize_cmd_list(t_var *var, t_token *tokens);
int		count_cmd_list(char **cmd_list);

/************* execute ****************/
int		execute_cmd_tok(t_var *var, t_token **tokens);

#endif
