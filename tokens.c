/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:13:54 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/20 13:16:39 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void print_token_type(t_token_type type) {
    switch (type) {
        case COMMAND:           printf("COMMAND"); break;
        case ARGUMENT:          printf("ARGUMENT"); break;
        case OPTION:         	printf("OPTION"); break;
        case PIPE:              printf("PIPE"); break;
        case REDIRECTION_LEFT:    printf("REDIRECTION_LEFT"); break;
        case REDIRECTION_RIGHT:   printf("REDIRECTION_RIGHT"); break;
        case APPEND:		printf("APPEND"); break;
        case HEREDOC:           printf("HEREDOC"); break;
        default:                printf("UNKNOWN"); break;
    }
}

void print_tokens(t_token *tokens) {
    int i = 0;
    while (tokens[i].value != NULL) {
        print_token_type(tokens[i].type);
        printf(": %s\n", tokens[i].value);
        i++;
    }
}
*/
int count_cmd_list(char **cmd_list)
{
	int count;

	count = 0;
	if (cmd_list == NULL)
		return 0;
	while (cmd_list[count] != NULL)
		count++;
	return count;
}

static void	create_token(char *cmd_part, t_token *tokens)
{
	if (!cmd_part)
		return ;
	if (cmd_part[0] == '-')
		tokens->type = OPTION;
	else if (is_builtins(cmd_part) == 10)
		tokens->type = PIPE;
	else if (is_builtins(cmd_part) == 11)
		tokens->type = REDIRECTION_RIGHT;
	else if (is_builtins(cmd_part) == 13)
		tokens->type = APPEND;
	else if (is_builtins(cmd_part) == 12)
		tokens->type = REDIRECTION_LEFT;
	else if (is_builtins(cmd_part) == 3)
		tokens->type = HEREDOC;
	else if (find_cmd_path(cmd_part) != NULL || is_builtins(cmd_part) != 0)
		tokens->type = COMMAND;
	else
		tokens->type = ARGUMENT;
	tokens->value = ft_strdup(cmd_part);
}

void	tokenize_cmd_list(t_var *var, t_token *tokens)
{
	int     i;

	i = 0;
	if (!tokens || !var || !var->cmd_list)
		return;
	while (var->cmd_list[i])
	{
		create_token(var->cmd_list[i], &tokens[i]);
		i++;
	}
	//free_list(var->cmd_list);
	tokens[i].type = UNKNOWN;
	tokens[i].value = NULL;
//	print_tokens(tokens);
}
