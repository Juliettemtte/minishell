/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:35:42 by arissane          #+#    #+#             */
/*   Updated: 2024/11/08 13:31:31 by jmouette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*copy_token(t_token *token)
{
	t_token	*copy;

	copy = malloc(sizeof(t_token));
	if (!copy)
		return (NULL);
	copy->type = token->type;
	if (token->type == HEREDOC)
		copy->heredoc_index = token->heredoc_index;
	copy->value = ft_strdup(token->value);
	if (!copy->value)
	{
		free(copy);
		return (NULL);
	}
	return (copy);
}

static t_token	**process_token_group(t_token *tokens, int *k, int *i)
{
	int		j;
	t_token	**token_group;

	token_group = malloc(sizeof(t_token *) * (*i - *k + 1));
	if (!token_group)
		return (NULL);
	j = 0;
	while (*k < *i)
	{
		token_group[j] = copy_token(&tokens[*k]);
		if (!token_group[j])
		{
			free(token_group);
			return (NULL);
		}
		*k = *k + 1;
		j++;
	}
	token_group[j] = NULL;
	return (token_group);
}

static int	group_by_pipes(t_token *tokens, t_token ***token_groups,
		int i, int *index)
{
	int	k;

	k = 0;
	while (tokens[i].value)
	{
		if (tokens[i].type == PIPE)
		{
			token_groups[*index] = process_token_group(tokens, &k, &i);
			if (!token_groups[*index])
				return (-1);
			(*index)++;
			k = i + 1;
		}
		i++;
	}
	if (i > k)
	{
		token_groups[*index] = process_token_group(tokens, &k, &i);
		if (!token_groups[*index])
			return (-1);
		(*index)++;
	}
	return (k);
}

t_token	***split_tokens(t_var *var, t_token *tokens)
{
	int		k;
	int		index;
	t_token	***token_groups;

	k = 0;
	index = 0;
	token_groups = malloc(sizeof(t_token **) * (var->pipes + 2));
	if (!token_groups)
		return (NULL);
	k = group_by_pipes(tokens, token_groups, 0, &index);
	if (k == -1)
	{
		free_token_groups(token_groups);
		return (NULL);
	}
	token_groups[index] = NULL;
	return (token_groups);
}
