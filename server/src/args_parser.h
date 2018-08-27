#pragma once
#include "s_smap_entry.h"

typedef enum {
	ARGS_PARSER_SUCCESS,
	ARGS_PARSER_SYSERROR,
	ARGS_PARSER_INVALID
} t_args_parser_status;

t_args_parser_status args_parser_parse(int argc, const char **argv, t_smap_entry **target_map);
