#pragma once
#include <stddef.h>
#include "s_smap_entry.h"

typedef enum {
	DSV_PARSER_SUCCESS,
	DSV_PARSER_SYSERROR,
	DSV_PARSER_INVALID,
	DSV_PARSER_INVALID_KEY,
	DSV_PARSER_EMPTY_LINE
} t_dsv_parser_status;

t_dsv_parser_status dsv_parser_parse(const char *buffer, t_smap_entry **target_map);
