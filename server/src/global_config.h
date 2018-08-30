#pragma once
#include "utils.h"
#include "log.h"

typedef struct {
	size_t rep_port;
	size_t pub_port;
	size_t cycle_interval;
	t_log_level log_level;
	char *log_file;
	char *config_file;
	size_t map_size;
} t_global_config;

t_global_config global_config;
