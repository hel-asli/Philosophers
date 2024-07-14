NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -fsanitize=address -g
RM = rm -f

SRCS = main.c 
OBJS = ${SRCS:.c=.o}

all : $(NAME)

%.o : %.c philo.h
	$(CC) $(CFLAGS) $< -c

$(NAME) :$(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

.PHONY : clean

