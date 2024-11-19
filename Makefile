NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -fsanitize=thread -lpthread
MFOLDER = Philo
RM = rm -f
MSRCS = $(MFOLDER)/main.c  $(MFOLDER)/parsing_utils.c $(MFOLDER)/utils.c
MOBJS = ${MSRCS:.c=.o}

all : $(MFOLDER)/$(NAME)

$(MFOLDER)/%.o : $(MFOLDER)/%.c $(MFOLDER)/philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MFOLDER)/$(NAME) : $(MOBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

fclean : clean
	$(RM) $(MFOLDER)/$(NAME)
clean :
	$(RM) $(MOBJS)

re : fclean all

.PHONY : clean