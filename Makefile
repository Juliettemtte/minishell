# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 11:23:04 by arissane          #+#    #+#              #
#    Updated: 2024/10/10 17:26:15 by jmouette         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Werror -Wextra

SRCS = main.c signals.c parse.c split_input.c redirect.c execute.c\
	utils.c pipes.c free_shell.c commands.c find_path.c tokens.c\
	redirections.c split_tokens.c\
	builtins/builtins_utils.c builtins/cd.c builtins/exit.c builtins/export.c\
	builtins/unset.c builtins/env.c builtins/echo.c builtins/pwd.c\
	builtins/heredoc.c

OFILES = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: LIBFT $(NAME)

LIBFT:
	@make -C $(LIBFT_DIR)

$(NAME): $(OFILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(OFILES) libft/libft.a -lreadline -o $(NAME) $(LIBFT)
	@ echo "Minishell compiled"

clean:
	@rm -f $(OFILES)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME) history.txt
	@make fclean -C $(LIBFT_DIR)
	@ echo "Minishell fclean"

re: fclean all

.PHONY: all clean fclean re libft
