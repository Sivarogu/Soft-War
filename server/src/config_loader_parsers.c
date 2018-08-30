#include "config_loader_parsers.h"
#include <string.h>
#include <stdlib.h>

int global_config_parse_rep_port(const char *str, t_global_config *config) {
	BIND_MTRUE(*str < '0' || *str > '9', "<rep-port> must be a positive integer");
	config->rep_port = atoi(str);
	return 0;
}

int global_config_parse_pub_port(const char *str, t_global_config *config) {
	BIND_MTRUE(*str < '0' || *str > '9', "<pub-port> must be a positive integer");
	config->pub_port = atoi(str);
	return 0;
}

int global_config_parse_cycle_interval(const char *str, t_global_config *config) {
	size_t cycle_interval;

	BIND_MTRUE(*str < '0' || *str > '9', "<cycle> must be a positive integer");
	BIND_MTRUE((cycle_interval = atoi(str) / 1000) < 1, "<cycle> must be greater or equal 1000");
	config->cycle_interval = cycle_interval;
	return 0;
}

int global_config_parse_log_file_path(const char *str, t_global_config *config) {
	config->log_file = strdup(str);
	return 0;
}

int global_config_parse_map_size(const char *str, t_global_config *config) {
	size_t map_size;

	BIND_MTRUE(*str < '0' || *str > '9', "<size> must be a positive integer");
	BIND_MTRUE((map_size = atoi(str)) < 5, "<size> must be greater or equal 5");
	config->map_size = map_size;
	return 0;
}

int global_config_parse_verbose_mode(const char *str, t_global_config *config) {
	config->log_level = LOG_LEVEL_INFO;
	return 0;
}
