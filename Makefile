NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
SRC_DIR = src
SRC = main.cpp \
      server.cpp \
      Client.cpp \
      Channel.cpp \
      ServerData.cpp \
      Message.cpp \
      AbstractCommand.cpp \
      Command/Pass.cpp \
      Command/Nick.cpp \
      Command/User.cpp

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -I./includes -c $< -o $@

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@printf "\e[38;5;82m\n  ${NAME} Compiled🚀\e[0m\n\n"

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f compile_commands.json

fclean: clean
	@rm -f $(NAME)
	@printf "\e[38;5;208m\n  successfully deleted🗑\e[0m\n\n"

re: fclean all

.PHONY: all clean fclean re
