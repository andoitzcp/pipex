# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acampo-p <acampo-p@student.42urduli>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 09:35:35 by acampo-p          #+#    #+#              #
#    Updated: 2023/01/19 09:44:01 by acampo-p         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Variables

NAME		= pipex
INCLUDE		= include/

LIBFT		= $(addprefix $(INCLUDE), libft/)
FT_PRINTF	= $(addprefix $(INCLUDE), ft_printf/)

LIBFT_A		= $(addprefix $(LIBFT), libft.a)
FT_PRINTF_A	= $(addprefix $(FT_PRINTF), libftprintf.a)

HEADERS     = src/pipex.h
SRC_DIR		= src/
OBJ_DIR		= obj/
CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -I$(INCLUDE)
RM			= rm -f

# Colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

#Sources

SRC_FILES	=	main utils debugging
BONUS_FILES	=	helloworld


SRC 		= 	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
BSRC 		= 	$(addprefix $(SRC_DIR), $(addsuffix .c, $(BONUS_FILES)))
OBJ 		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
BOBJ 		= 	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(BONUS_FILES)))

###

OBJF		=	.cache_exists

all:		$(NAME)

$(NAME):	$(OBJ) $(LIBFT_A) $(FT_PRINTF_A) $(HEADERS)
			@$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT) -lft -L$(FT_PRINTF) -lftprintf -o $(NAME) -lXext -lX11
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

bonus:		$(BOBJ) $(LIBFT_A) $(FT_PRINTF_A) $(HEADERS)
			@$(CC) $(CFLAGS) $(BOBJ) -L$(LIBFT) -lft -L$(FT_PRINTF) -lftprintf -o $(NAME) -lXext -lX11
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

$(LIBFT_A):
			@$(MAKE) -s -C $(LIBFT)
			@echo "Compiled $(LIBFT_A)"
$(FT_PRINTF_A):
			@$(MAKE) -s -C $(FT_PRINTF)
			@echo "Compiled $(FT_PRINTF_A)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJF)
			@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJF):
			@mkdir -p $(OBJ_DIR)

clean:
			@$(RM) -rf $(OBJ_DIR)
			@$(MAKE) clean -C $(LIBFT)
			@$(MAKE) clean -C $(FT_PRINTF)
			@$(MAKE) clean -C $(MLX)
			@echo "$(BLUE)$(NAME) object files succesfully cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@echo "$(CYAN)$(NAME) executable files succesfully cleaned!$(DEF_COLOR)"
			@$(RM) -f $(LIBFT)/$(LIBFT_A)
			@echo "$(CYAN)$(LIBFT) executable files succesfully cleaned!$(DEF_COLOR)"
			@$(RM) -f $(FT_PRINTF)/$(FT_PRINTF_A)
			@echo "$(CYAN)$(FT_PRINTF) executable files succesfully cleaned!$(DEF_COLOR)"

re:			fclean all
			@echo "$(GREEN)Everything was cleaned and the rebuilt for pipex!$(DEF_COLOR)"

norm:
			@norminette $(SRC) $(INCLUDE) $(LIBFT) $(FT_PRINTF) | grep -v Norme -B1 || true
