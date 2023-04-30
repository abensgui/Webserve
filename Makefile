NAME = webserv

CC = c++ -std=c++98

FLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

SRC = main.cpp configuration_file/configuration.cpp  Server/socketServer.cpp  response/GetResponse.cpp

all : $(NAME)
$(NAME) : $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean :
	rm -f $(NAME)

fclean :
	rm -f $(NAME)

re : fclean all