NAME := ft_ls
LIBFT_DIR := libft
LIBFT_A := $(LIBFT_DIR)/libft.a

CC := cc
CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -Iinclude -I$(LIBFT_DIR)

SRC_DIR := src
OBJ_DIR := obj

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/cli/parse_args.c \
	$(SRC_DIR)/cli/print_help.c \
	$(SRC_DIR)/core/run_ft_ls.c \
	$(SRC_DIR)/fs/list_path.c \
	$(SRC_DIR)/util/config.c

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
