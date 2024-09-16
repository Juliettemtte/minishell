# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 11:23:04 by arissane          #+#    #+#              #
#    Updated: 2024/09/16 17:52:56 by jmouette         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Werror -Wextra

SRCS = main.c signals.c parse.c split_input.c builtins.c redirect.c utils.c pipes.c\
	tokens.c

OFILES = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OFILES)
	@$(CC) $(CFLAGS) $(OFILES) libft/libft.a -lreadline -o $(NAME)
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
