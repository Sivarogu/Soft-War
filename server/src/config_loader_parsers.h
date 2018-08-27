#pragma once
#include "global_config.h"

int global_config_parse_rep_port(const char *str, t_global_config *config);
int global_config_parse_pub_port(const char *str, t_global_config *config);
int global_config_parse_cycle_interval(const char *str, t_global_config *config);
int global_config_parse_log_file_path(const char *str, t_global_config *config);
int global_config_parse_map_size(const char *str, t_global_config *config);
int global_config_parse_verbose_mode(const char *str, t_global_config *config);
