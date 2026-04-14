CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv

SRC_DIR = src
OBJ_DIR = obj

INCLUDES = -I includes

SRCS = src/main.cpp\
	   src/server/Server.cpp\
	   src/server/ServerCommands.cpp\
	   src/client/Client.cpp\
	   src/utils/MessageParser.cpp\
	   src/utils/SignalHandler.cpp

HDRS = includes/Server.hpp\
	includes/Client.hpp\
	includes/MessageParser.hpp\
	includes/SignalHandler.hpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CXXFLAGS += -g -fsanitize=address
debug: re

.PHONY: all clean fclean re debug