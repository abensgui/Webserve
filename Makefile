NAME = webserv

CC = c++ -std=c++98

FLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

SRC = configuration_file/configuration.cpp Request/request.cpp main.cpp Server/socketServer.cpp response/ft_Response.cpp

all : $(NAME)
$(NAME) : $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean :
	rm -f $(NAME)

fclean :
	rm -f $(NAME)

re : fclean all