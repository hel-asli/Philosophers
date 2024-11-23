NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = #-fsanitize=thread -lpthread
RM = rm -f
MSRCS = main.c parsing_utils.c utils.c philo_utils.c \
		data_init.c monitor.c encapsulate_mutex.c \
		philo_create.c	
MOBJS = ${MSRCS:.c=.o}

all : $(NAME)

%.o : %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(MOBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean :
	$(RM) $(MOBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : clean
