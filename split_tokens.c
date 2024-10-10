/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:35:42 by arissane          #+#    #+#             */
/*   Updated: 2024/10/09 10:47:42 by arissane         ###   ########.fr       */
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
			printf("token %d value = %s type = %d\n", j, token_groups[i][j]->value, token_groups[i][j]->type);
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
	copy->value = ft_strdup(token->value);
	if (!copy->value)
	{
		free(copy);
		return (NULL);
	}
	return (copy);
}

t_token	***split_tokens(t_var *var, t_token *tokens)
{
	int		i;
	int		j;
	int		k;
	int		group_index;
	t_token	***token_groups;

	i = 0;
	k = 0;
	group_index = 0;
	token_groups = malloc(sizeof(t_token **) * (var->pipes + 2));
	if (!token_groups)
		return (NULL);
	while (tokens[i].value)
	{
		if (tokens[i].type == PIPE)
		{
			token_groups[group_index] = malloc(sizeof(t_token *) * (i - k + 1));
			if (!token_groups[group_index])
			{
				free_token_groups(token_groups);
				return (NULL);
			}
			j = 0;
			while (k < i)
			{
				token_groups[group_index][j] = copy_token(&tokens[k]);
				if (!token_groups[group_index][j])
				{
					free_token_groups(token_groups);
					return (NULL);
				}
				k++;
				j++;
			}
			token_groups[group_index][j] = NULL;
			group_index++;
			k = i + 1;
		}
		i++;
	}
	//copy the final group of tokens after a pipe or if there is no pipe
	if (i > k)
	{
		token_groups[group_index] = malloc(sizeof(t_token *) * (i - k + 1));
		if (!token_groups[group_index])
		{
			free_token_groups(token_groups);
			return (NULL);
		}
		j = 0;
		while (k < i)
		{
			token_groups[group_index][j] = copy_token(&tokens[k]);
			if (!token_groups[group_index][j])
			{
				free_token_groups(token_groups);
				return (NULL);
			}
			k++;
			j++;
		}
		token_groups[group_index][j] = NULL;
		group_index++;
	}
	token_groups[group_index] = NULL;
//	print_token_groups(token_groups);
	return (token_groups);
}
