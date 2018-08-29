#
# Makefile for piscine in /home/tassel_p/piscine
#
# Made by TASSEL Pierre
# Login   <tassel_p@etna-alternance.net>
#
# Started on  Tue Jan 18 10:00:00 2017 TASSEL Pierre
# Last update Tue Jan 18 10:00:00 2017 TASSEL Pierre
#

# program
PROGRAM_NAME		=	softwar-server

# sources
SOURCE_DIR			=	src

SOURCE_SUB_DIRS		=


SOURCE_INCLUDES		=	main.c \
						action.c \
						args_parser.c \
						command.c \
						config_loader.c \
						config_loader_parsers.c \
						dsv_parser.c \
						game.c \
						global_config.c \
						log.c \
						network.c \
						thread_publish.c \
						utils.c


SOURCE_FILES		=	$(addprefix $(SOURCE_DIR)/,$(SOURCE_INCLUDES))

# build
BUILD_DIR			=	dist
BUILD_TARGET		=	$(BUILD_DIR)/$(PROGRAM_NAME)
BUILD_SUB_DIRS		=	$(addprefix $(BUILD_DIR)/,$(SOURCE_SUB_DIRS))
BUILD_OBJECTS		=	$(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))

# compiler
COMPILER_NAME		=	gcc
COMPILER_FLAGS		=	-Wfatal-errors -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-parameter -pedantic -std=gnu99 -D_GNU_SOURCE -g -c

LINKER_NAME			=	gcc
LINKER_FLAGS		=	-Wall -Wextra -Wpedantic -Werror -pedantic -lm -g
LINKER_LIBS			=	-lczmq \
						-lzmq \
						-ljansson \
						-pthread

# shorthands
_MAKE				=	make
_RM					=	rm -f
_RM_DIR				=	rm -rf
_WATCHER			=	nodemon
_MKDIR				=	mkdir -p
_WATCH				=	$(_WATCHER) --watch "$(SOURCE_DIR)" --ext c,h
_LD					=	$(LINKER_NAME) $(LINKER_FLAGS) $(LINKER_LIBS)
_CC					=	$(COMPILER_NAME) $(COMPILER_FLAGS)

# rules

.PHONY: all clean fclean re run watch \
	__watch

all: $(BUILD_TARGET)

$(BUILD_TARGET): $(BUILD_OBJECTS)
	$(_LD) $(BUILD_OBJECTS) -o $(BUILD_TARGET)

$(BUILD_OBJECTS): | $(BUILD_DIR) $(BUILD_SUB_DIRS)

$(BUILD_DIR):
	$(_MKDIR) $(BUILD_DIR)

$(BUILD_SUB_DIRS):
	$(_MKDIR) $(BUILD_SUB_DIRS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(_CC) $< -o $@

clean:
	$(_RM_DIR) $(BUILD_OBJECTS)

fclean: clean
	$(_RM_DIR) $(BUILD_TARGET)
	$(_RM_DIR) $(BUILD_DIR)

re: fclean all

run:
	$(BUILD_TARGET) --cycle 1000000 --log dist/logs.txt --size 5

watch:
	$(_MAKE) -s __watch

__watch:
	which $(_WATCHER) > /dev/null \
		&& $(_WATCH) --exec "$(_MAKE) -s re && $(_MAKE) -s run || exit 1" \
		|| printf "\\033[31mPlease install $(_WATCHER)\\033[0m: sudo npm i -g $(_WATCHER)\\n"
