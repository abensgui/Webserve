NAME = webserv

CC = c++ -std=c++98

FLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

SRC =	Request/request.cpp\
		configuration_file/configuration.cpp\
		main.cpp\
		Server/socketServer.cpp\
		response/ft_Response.cpp\
		response/statut_code.cpp\
		response/get_content_type.cpp\
		response/ft_get.cpp\
		response/ft_post.cpp\
		response/ft_delete.cpp\

all : $(NAME)
$(NAME) : $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

clean :
	rm -f $(NAME)

fclean :
	rm -f $(NAME)

re : fclean all