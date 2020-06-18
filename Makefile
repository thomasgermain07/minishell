# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thgermai <thgermai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/09 08:26:20 by thgermai          #+#    #+#              #
#    Updated: 2020/06/18 11:49:26 by thgermai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCSDIR = srcs
OBJSDIR = .objs
SRCS = main.c\
		pipe_parsing.c\
		redirection.c
OBJS = $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DPDCS = $(OBJS:.o=.d)
INCLUDES = -I includes/ -I libft/
LIB = libft/libft.a
CFLAGS = -Wall -Wextra -Werror
LOGFILE = $(LOGPATH) `date +'%y.%m.%d %H:%M:%S'`
MSG = ---

opti :
	@(make -j all)

all : $(NAME)

-include $(DPDCS)

$(NAME) : $(LIB) $(OBJS)
	@(gcc $(CFLAGS) $(OBJS) $(LIB) $(INCLUDES) -o $(NAME))
	@(echo "$(NAME) created")

$(LIB) :
	@(make -C libft)

$(OBJSDIR)/%.o : $(SRCSDIR)/%.c | $(OBJSDIR)
	@(echo "Compiling -> $^")
	@(gcc $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@)

$(OBJSDIR) :
	@(mkdir -p .objs)

clean :
	@(rm -f $(NAME))

fclean : clean
	@(rm -rf $(OBJSDIR))

git : fclean
	@(make fclean -C libft)
	@(git add *)
	@(git commit -m "$(LOGFILE): $(MSG)")
	@(git push)

re : fclean all
