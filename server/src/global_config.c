#include "global_config.h"

t_global_config global_config = {
	.rep_port = 4242,
	.pub_port = 4243,
	.cycle_interval = 500000,
	.log_level = LOG_LEVEL_WARN,
	.log_file = NULL,
	.config_file = NULL,
	.map_size = 5
};
