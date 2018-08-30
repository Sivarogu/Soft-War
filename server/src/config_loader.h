#pragma once
#include <stdbool.h>
#include "global_config.h"
#include "s_smap_entry.h"

typedef struct {
	char *name;
	int (*parser)(const char *str);
	bool is_switch;
	bool is_required;
} t_global_config_parser;

int global_config_of_config_file();
int global_config_of_args(int argc, const char **argv);
int global_config_of_smap(t_smap_entry *parsed_config, const t_global_config_parser *parsers);
