NAME		:= webserv

CC			:= clang++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98
DEP_FLAGS	:= -MP -MMD

SRCS_DIR	:= sources
HDRS_DIR	:= headers
vpath %.cpp	$(SRCS_DIR)
vpath %.hpp	$(HDRS_DIR)
INCLUDES	:= -I$(HDRS_DIR)

# why need HDRS?

HDRS		:= Example1.class.hpp \
			example2.hpp

SRCS		:= main.cpp \
			Example1.class.cpp \
			example.cpp

OBJS_DIR	:= .objects
OBJS		:= $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

DEPS		:= $(SRCS:%.cpp=$(OBJS_DIR)/%.d)

all:
					@echo "$(BOLD)Creating/updating $(WHITE_B)'$(NAME)'$(BOLD):$(DEF)"
					@$(MAKE) --no-print-directory $(NAME)

$(NAME):			$(OBJS)
					@echo "$(BOLD)Linking files...$(DEF)"
					@$(CC) $(OBJS) $(CFLAGS) $(INCLUDES) -o $@
					@echo "$(WHITE_B)'$(NAME)'$(BOLD) has been created/updated.$(DEF)"

$(OBJS_DIR)/%.o:	%.cpp | $(OBJS_DIR) 
					@echo "Assembling $<..."
					@$(CC) $(CFLAGS) $(DEP_FLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
					@mkdir -p $@
					@echo "$(BOLD)Directory '$(OBJS_DIR)' has been created.$(DEF)"

clean:
					@rm -rf $(OBJS) $(DEPS) $(OBJS_DIR)
					@echo "$(WHITE)$(NAME): $(BOLD)Object files have been cleaned.$(DEF)"

fclean:				clean
					@rm -rf $(NAME)
					@echo "$(WHITE)'$(NAME)'$(BOLD) has been cleaned.$(DEF)"

re:					fclean all

.PHONY:				all clean fclean re

-include $(DEPS)

BOLD	:= \033[0;1m
WHITE_B	:= \033[1;37m
WHITE	:= \033[37m
DEF		:= \033[0;39m