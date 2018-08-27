#pragma once
#include <stdbool.h>
#include "global_config.h"
#include "s_smap_entry.h"

typedef struct {
	char *name;
	int (*parser)(const char *str, t_global_config *config);
	bool is_switch;
	bool is_required;
} t_global_config_parser;

int global_config_of_file(const char *path, t_global_config *config);
int global_config_of_args(int argc, const char **argv, t_global_config *config);
int global_config_of_smap(t_smap_entry *parsed_config_it, t_global_config *config);
