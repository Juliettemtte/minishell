/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:34:15 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/16 17:52:20 by jmouette         ###   ########.fr       */
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

typedef enum e_token_type
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECTION_IN,
	REDIRECTION_OUT,
	REDIRECTION_APPEND,
	HEREDOC,
	FILENAME,
	UNKNOWN
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

typedef struct s_var
{
	char		*input;
	char		**cmd_list;
	char		*heredoc;
}	t_var;

/*************** main ****************/

/************** signal ***************/
void	init_signal(void);

/*************** parse ***************/
int		parse(t_var *variables);

/*************** pipes ***************/
char	*find_cmd_path(char *cmd);

/************** tokens ***************/
void	tokenize_cmd_list(t_var *var);

/************ split_input ************/
char	**split_input(char const *s, char c);

/*************** utils ***************/
int		check_quotes(char *input);
int		check_format(char *arg);
void	ft_swap(char **a, char **b);
void	bubble_sort(char **arr, int n);
void	free_list(char **list);

int	ft_strcmp(const char *s1, const char *s2);
int	is_builtins(char *cmd);

/**************redirect***************/
void	redirectreplace_output_right(char *content, char *target);
void	redirect_output_right(char *content, char *target);

/************* builtins **************/
void	handle_cd(char *dest);
void	handle_env(void);
void	handle_export(char *name, char *value);
int		handle_unset(char *name, size_t name_len);
void	print_env_sorted(void);

#endif
