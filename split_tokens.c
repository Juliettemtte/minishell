/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:35:42 by arissane          #+#    #+#             */
/*   Updated: 2024/10/18 10:22:27 by arissane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	print_token_groups(t_token ***token_groups)
{
	int	i;
	int	j;

	i = 0;
	while (token_groups[i])
	{
		printf("\nToken group %d:\n", i);
		j = 0;
		while (token_groups[i][j])
		{
			printf("token %d value = %s type = %d\n", 
			j, token_groups[i][j]->value, token_groups[i][j]->type);
			j++;
		}
		i++;
	}
	printf("\n");
}*/

t_token	*copy_token(t_token *token)
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

t_token	**process_token_group(t_token *tokens, int *k, int *i)
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

int	group_by_pipes(t_token *tokens, int *i, t_token ***token_groups, int *j)
{
	int	k;

	k = 0;
	while (tokens[*i].value)
	{
		if (tokens[*i].type == PIPE)
		{
			token_groups[*j] = process_token_group(tokens, &k, i);
			if (!token_groups[*j])
				return (-1);
			(*j)++;
			k = *i + 1;
		}
		(*i)++;
	}
	if (*i > k)
	{
		token_groups[*j] = process_token_group(tokens, &k, i);
		if (!token_groups[*j])
		{
			free_token_groups(token_groups);
			return (-1);
		}
		(*j)++;
	}
	return (k);
}

t_token	***split_tokens(t_var *var, t_token *tokens)
{
	int		i;
	int		k;
	int		group_index;
	t_token	***token_groups;

	i = 0;
	k = 0;
	group_index = 0;
	token_groups = malloc(sizeof(t_token **) * (var->pipes + 2));
	if (!token_groups)
		return (NULL);
	k = group_by_pipes(tokens, &i, token_groups, &group_index);
	if (k == -1)
	{
		free_token_groups(token_groups);
		return (NULL);
	}
	token_groups[group_index] = NULL;
	return (token_groups);
}
