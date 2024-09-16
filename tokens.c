/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:13:54 by jmouette          #+#    #+#             */
/*   Updated: 2024/09/16 17:57:10 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_token_type(t_token_type type) {
    switch (type) {
        case COMMAND:           printf("COMMAND"); break;
        case ARGUMENT:          printf("ARGUMENT"); break;
        case PIPE:              printf("PIPE"); break;
        case REDIRECTION_IN:    printf("REDIRECTION_IN"); break;
        case REDIRECTION_OUT:   printf("REDIRECTION_OUT"); break;
        case REDIRECTION_APPEND:printf("REDIRECTION_APPEND"); break;
        case HEREDOC:           printf("HEREDOC"); break;
        case FILENAME:          printf("FILENAME"); break;
        default:                printf("UNKNOWN"); break;
    }
}

void print_tokens(t_token **tokens) {
    int i = 0;
    while (tokens[i] != NULL) {
        print_token_type(tokens[i]->type);
        printf(": %s\n", tokens[i]->value);
        i++;
    }
}


static int count_cmd_list(char **cmd_list)
{
	int count;

	count = 0;
	if (cmd_list == NULL)
		return 0;
	while (cmd_list[count] != NULL)
		count++;
	return count;
}

static t_token	*create_token(char *cmd_part)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (ft_strcmp(cmd_part, "|") == 0)
		token->type = PIPE;
	else if (ft_strcmp(cmd_part, ">") == 0)
		token->type = REDIRECTION_OUT;
	else if (ft_strcmp(cmd_part, ">>") == 0)
		token->type = REDIRECTION_APPEND;
	else if (ft_strcmp(cmd_part, "<") == 0)
		token->type = REDIRECTION_IN;
	else if (ft_strcmp(cmd_part, "<<") == 0)
		token->type = HEREDOC;
	else if (find_cmd_path(cmd_part) != NULL || is_builtins(cmd_part) != 0)
		token->type = COMMAND;
	else
		token->type = ARGUMENT;
	token->value = ft_strdup(cmd_part);
	return (token);
}

void	tokenize_cmd_list(t_var *var)
{
	t_token	**tokens;
	int     i;

	tokens = malloc(sizeof(t_token *) * (count_cmd_list(var->cmd_list) + 1));
	if (!tokens)
		return ;
	i = 0;
	while (var->cmd_list[i])
	{
		tokens[i] = create_token(var->cmd_list[i]);
		i++;
	}
	tokens[i] = NULL;
	print_tokens(tokens);
}
