#include "config_loader.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "config_loader_parsers.h"
#include "args_parser.h"
#include "dsv_parser.h"

#define READ_FILE_SIZE 1024
#define CONFIG_FILE_MAX_SIZE READ_FILE_SIZE - 1

static const t_global_config_parser global_config_parsers_file[] = {
	{.name = "rep-port", .parser = global_config_parse_rep_port, .is_switch = false, .is_required = false},
	{.name = "pub-port", .parser = global_config_parse_pub_port, .is_switch = false, .is_required = false},
	{.name = "cycle", .parser = global_config_parse_cycle_interval, .is_switch = false, .is_required = false},
	{.name = "log", .parser = global_config_parse_log_file_path, .is_switch = false, .is_required = true},
	{.name = "size", .parser = global_config_parse_map_size, .is_switch = false, .is_required = true},
{.name = NULL}};

static const t_global_config_parser global_config_parsers_cli[] = {
	{.name = "rep-port", .parser = global_config_parse_rep_port, .is_switch = false, .is_required = false},
	{.name = "pub-port", .parser = global_config_parse_pub_port, .is_switch = false, .is_required = false},
	{.name = "cycle", .parser = global_config_parse_cycle_interval, .is_switch = false, .is_required = false},
	{.name = "log", .parser = global_config_parse_log_file_path, .is_switch = false, .is_required = true},
	{.name = "size", .parser = global_config_parse_map_size, .is_switch = false, .is_required = true},
	{.name = "config-file", .parser = global_config_parse_config_file_path, .is_switch = false, .is_required = false},
	{.name = "v", .parser = global_config_parse_verbose_mode, .is_switch = true, .is_required = false},
{.name = NULL}};

int global_config_of_smap(t_smap_entry *parsed_config, const t_global_config_parser *parsers) {
	const t_global_config_parser *parser_it;
	const t_smap_entry *parsed_config_it;

	parser_it = parsers;
	while (parser_it->name) {
		if (parser_it->is_required) {
			parsed_config_it = parsed_config;
			while (parsed_config_it->key && strcmp(parsed_config_it->key, parser_it->name) != 0)
				parsed_config_it++;
			BIND_MZERO(parsed_config_it->key, "missing required parameter <%s>", parser_it->name);
		}
		parser_it++;
	}
	parsed_config_it = parsed_config;
	while (parsed_config_it->key) {
		parser_it = parsers;
		while (parser_it->name && strcmp(parsed_config_it->key, parser_it->name) != 0)
			parser_it++;
		BIND_MZERO(parser_it->parser, "unknown parameter <%s>", parsed_config_it->key);
		BIND_MTRUE(!parsed_config_it->value && !parser_it->is_switch, "missing parameter <%s> value", parsed_config_it->key)
		BIND_NEG(parser_it->parser(parsed_config_it->value));
		parsed_config_it++;
	}
	return 0;
}

int global_config_of_config_file() {
	int config_fd;
	char buffer[READ_FILE_SIZE];
	int read_length;
	t_smap_entry *parsed_config;

	if (!global_config.config_file)
		return 0;
	BIND_MNEG((config_fd = open(global_config.config_file, O_RDONLY)), "failed opening config file");
	BIND_MNEG((read_length = read(config_fd, buffer, CONFIG_FILE_MAX_SIZE)), "failed reading config file");
	BIND_MTRUE(read_length > CONFIG_FILE_MAX_SIZE, "config file is too big");
	BIND_MNEG(close(config_fd), "failed closing config file");
	buffer[read_length - 1] = '\0';
	BIND_MTRUE(dsv_parser_parse(buffer, &parsed_config) != DSV_PARSER_SUCCESS, "failed parsing config buffer");
	BIND_MNEG(global_config_of_smap(parsed_config, global_config_parsers_file), "failed parsing config contents");
	return 0;
}

int global_config_of_args(int argc, const char **argv) {
	t_smap_entry *parsed_config;

	BIND_TRUE(args_parser_parse(argc, argv, &parsed_config) != ARGS_PARSER_SUCCESS);
	BIND_NEG(global_config_of_smap(parsed_config, global_config_parsers_cli));
	return 0;
}
