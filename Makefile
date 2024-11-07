NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -fsanitize=thread -g
MFOLDER = Mandatory
RM = rm -f
MSRCS = $(MFOLDER)/main.c 
MOBJS = ${MSRCS:.c=.o}

all : $(MFOLDER)/$(NAME)

$(MFOLDER)/%.o : $(MFOLDER)/%.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MFOLDER)/$(NAME) : $(MOBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(RM) $(NAME)
	@ln -s $@ $(NAME)

clean :
	$(RM) $(MOBJS)

fclean : clean
	$(RM) $(NAME) $(MFOLDER)/$(NAME)

re : fclean all

.PHONY : clean
