NAME		:= webserv

CC			:= clang++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98
DEP_FLAGS	:= -MP -MMD

SRCS_DIRS	:= $(addprefix sources, \
                    / \
                    /utils \
                )
HDRS_DIRS	:= $(addprefix headers, \
                    / \
                    /utils \
                )

vpath %.cpp	$(SRCS_DIRS)
vpath %.hpp	$(HDRS_DIRS)

# why need HDRS?
# HDRS		:= Example1.class.hpp \
# 			example2.hpp

SRCS        :=  main.cpp \
                $(addprefix utils/,\
                    logger.cpp \
                )
                # $(addprefix example_dir/,
                #     example1.cpp,
                #     example2.cpp
                # )

OBJS_DIR	:= .objects
OBJS		:= $(addprefix $(OBJS_DIR)/, \
				$(notdir $(patsubst %.cpp, %.o, $(SRCS))))

DEPS		:= $(addprefix $(OBJS_DIR)/, \
				$(notdir $(patsubst %.cpp, %.d, $(SRCS))))


all:
					@echo -e "$(FMT_BOLD)Creating/updating $(FMT_WHITE_B)'$(NAME)'$(FMT_BOLD):$(FMT_DEF)"
					@$(MAKE) --no-print-directory $(NAME)

$(NAME):			$(OBJS)
					@echo -e "$(FMT_BOLD)Linking files...$(FMT_DEF)"
					@$(CC) $(OBJS) $(CFLAGS) -o $@
					@echo -e "$(FMT_WHITE_B)'$(NAME)'$(FMT_BOLD) has been created/updated.$(FMT_DEF)"

$(OBJS_DIR)/%.o:	%.cpp | $(OBJS_DIR)
					@echo -e "Assembling $<..."
					@$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@

$(OBJS_DIR):
					@mkdir -p $@
					@echo -e "$(FMT_BOLD)Directory '$(OBJS_DIR)' has been created.$(FMT_DEF)"

clean:
					@rm -rf $(OBJS) $(DEPS) $(OBJS_DIR)
					@echo -e "$(FMT_WHITE)$(NAME): $(FMT_BOLD)Object files have been cleaned.$(FMT_DEF)"

fclean:				clean
					@rm -rf $(NAME)
					@echo -e "$(FMT_WHITE)'$(NAME)'$(FMT_BOLD) has been cleaned.$(FMT_DEF)"

re:					fclean all

.PHONY:				all clean fclean re

-include $(DEPS)

FMT_BOLD	:= \033[0;1m
FMT_WHITE_B	:= \033[1;37m
FMT_WHITE	:= \033[37m
FMT_DEF		:= \033[0;39m
