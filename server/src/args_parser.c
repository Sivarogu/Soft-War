#include "args_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "log.h"

t_args_parser_status args_parser_parse(int argc, const char **argv, t_smap_entry **target_map) {
	const char **argv_end_it;
	t_smap_entry *map_it;

	argv_end_it = argv + argc;
	argv++;
	BIND_ZERO((*target_map = map_it = malloc(argc * sizeof(t_smap_entry))));
	while (argv < argv_end_it) {
		if (strncmp(*argv, "--", 2) != 0) {
			log_error("expected option key to be prefixed by \"--\", found \"%s\"\n", *argv);
			return ARGS_PARSER_INVALID;
		}
		if (argv + 1 >= argv_end_it) {
			log_error("expected value after option \"%s\"\n", *argv);
			return ARGS_PARSER_INVALID;
		}
		if (!(map_it->key = strdup(*argv + 2))
			|| !(map_it->value = strdup(argv[1])))
			return ARGS_PARSER_SYSERROR;
		map_it++;
		argv += 2;
	}
	map_it->key = map_it->value = NULL;
	return ARGS_PARSER_SUCCESS;
}
