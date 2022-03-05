NAME		:= webserv

CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98
DEP_FLAGS	:= -MP -MMD

SRCS_DIRS	:= $(addprefix sources, \
					/ \
		  			/core \
					/cgi \
					/utils \
					/config \
					/http \
				)

HDRS_DIRS	:= $(addprefix headers, \
					/ \
		  			/core \
					/utils \
					/cgi \
					$(addprefix /config, \
						/models \
						/parser \
					) \
					$(addprefix /http, \
						/models \
						/parser \
					) \
				)

vpath %.cpp	$(SRCS_DIRS)
vpath %.hpp	$(HDRS_DIRS)

SRCS		:=  main.cpp \
				$(addprefix cgi/,\
					cgi.cpp \
				) \
				$(addprefix config/, \
					parser.cpp \
				) \
				$(addprefix core/,\
					core.cpp \
					requesthandler.cpp \
					pagegenerator.cpp \
				) \
				$(addprefix http/, \
					parser.cpp \
				) \
				$(addprefix utils/,\
					logger.cpp \
					file.cpp \
					debug.cpp \
					time.cpp \
					string.cpp \
					exception.cpp \
				)
				# $(addprefix example_dir/,
				#	 example1.cpp,
				#	 example2.cpp
				# )

OBJS_DIR	:= .objects
OBJS		:= $(addprefix $(OBJS_DIR)/, \
				$(patsubst %.cpp, %.o, $(SRCS)))

DEPS		:= $(addprefix $(OBJS_DIR)/, \
				$(patsubst %.cpp, %.d, $(SRCS)))


all:
					@echo "$(FMT_BOLD)Creating/updating $(FMT_WHITE_B)'$(NAME)'$(FMT_BOLD):$(FMT_DEF)"
					@$(MAKE) --no-print-directory $(NAME)

$(NAME):			$(OBJS)
					@echo "$(FMT_BOLD)Linking files...$(FMT_DEF)"
					@$(CC) $(OBJS) $(CFLAGS) -o $@
					@echo "$(FMT_WHITE_B)'$(NAME)'$(FMT_BOLD) has been created/updated.$(FMT_DEF)"

$(OBJS_DIR)/%.o:	%.cpp | $(OBJS_DIR)
					@echo "Assembling $<..."
					@mkdir -p $(dir $@)
					@$(CC) $(CFLAGS) $(DEP_FLAGS) -c $< -o $@

$(OBJS_DIR):
					@mkdir -p $@
					@echo "$(FMT_BOLD)Directory '$(OBJS_DIR)' has been created.$(FMT_DEF)"

clean:
					@rm -rf $(OBJS) $(DEPS) $(OBJS_DIR)
					@echo "$(FMT_WHITE)$(NAME): $(FMT_BOLD)Object files have been cleaned.$(FMT_DEF)"

fclean:				clean
					@rm -rf $(NAME)
					@echo "$(FMT_WHITE)'$(NAME)'$(FMT_BOLD) has been cleaned.$(FMT_DEF)"

re:					fclean all

.PHONY:				all clean fclean re

-include $(DEPS)

FMT_BOLD	:= \033[0;1m
FMT_WHITE_B	:= \033[1;37m
FMT_WHITE	:= \033[37m
FMT_DEF		:= \033[0;39m
