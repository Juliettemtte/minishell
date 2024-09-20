/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:34:15 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/20 13:04:46 by arissane         ###   ########.fr       */
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
	char			*value;
}	t_token;

typedef struct s_var
{
	char		*input;
	char		**cmd_list;
	char		*heredoc;
}	t_var;

/*************** main ****************/

/*************** check ***************/
int		check_quotes(char *input);
int		check_format(char *arg);
int		check_command_syntax(t_token *tokens);

/**************** free ***************/
void	free_list(char **list);

/************** signal ***************/
void	init_signal(void);

/*************** parse ***************/
int		parse(t_var *variables);
int		run_command(t_var *var);

/*************** pipes ***************/
char	*find_cmd_path(char *cmd);

/************** tokens ***************/
int		count_cmd_list(char **cmd_list);
void	tokenize_cmd_list(t_var *var, t_token *tokens);

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
void	handle_cd(char *dest);
void	handle_env(void);
void	handle_export(char *name, char *value);
int		handle_unset(char *name, size_t name_len);
void	print_env_sorted(void);

/************ builtins2 **************/
int		handle_heredoc(t_var *var);
int		handle_pwd(t_var *var);
int		handle_echo(char **cmd_list);

#endif
