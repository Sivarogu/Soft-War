#include "dsv_parser.h"
#include <stdlib.h>
#include <string.h>

static size_t dsv_parser_count_filled_lines(const char *buffer) {
	size_t line_count;
	const char *buffer_it;

	line_count = 0;
	buffer_it = buffer;
	while (*buffer_it) {
		if (buffer_it != buffer && *buffer_it == '\n' && buffer_it[-1] != '\n')
			line_count++;
		buffer_it++;
	}
	if (buffer_it != buffer && buffer_it[-1] != '\n')
		line_count++;
	return line_count;
}

static t_dsv_parser_status dsv_parser_parse_key(const char **buffer_it_ref, char **target_str) {
	const char *buffer_it;

	buffer_it = *buffer_it_ref;
	while (*buffer_it && *buffer_it != ':' && *buffer_it != '\n')
		buffer_it++;
	if (buffer_it == *buffer_it_ref) {
		*buffer_it_ref = buffer_it + 1;
		return *buffer_it == ':' ? DSV_PARSER_INVALID_KEY : DSV_PARSER_EMPTY_LINE;
	}
	if (*buffer_it != ':') {
		*buffer_it_ref = buffer_it;
		return DSV_PARSER_INVALID;
	}
	if (!(*target_str = strndup(*buffer_it_ref, buffer_it - *buffer_it_ref)))
		return DSV_PARSER_SYSERROR;
	(*target_str)[buffer_it - *buffer_it_ref] = '\0';
	*buffer_it_ref = buffer_it + 1;
	return DSV_PARSER_SUCCESS;
}

static t_dsv_parser_status dsv_parser_parse_value(const char **buffer_it_ref, char **target_str) {
	const char *buffer_it;

	buffer_it = *buffer_it_ref;
	while (*buffer_it && *buffer_it != '\n')
		buffer_it++;
	if (!(*target_str = strndup(*buffer_it_ref, buffer_it - *buffer_it_ref)))
		return DSV_PARSER_SYSERROR;
	(*target_str)[buffer_it - *buffer_it_ref] = '\0';
	*buffer_it_ref = buffer_it + 1;
	return DSV_PARSER_SUCCESS;
}

t_dsv_parser_status dsv_parser_parse(const char *buffer, t_smap_entry **target_map) {
	const char *buffer_it;
	size_t line_count;
	t_smap_entry *result_map_it;
	t_dsv_parser_status dsv_status;

	line_count = dsv_parser_count_filled_lines(buffer);
	if (!(result_map_it = *target_map = malloc((line_count + 1) * sizeof(t_smap_entry))))
		return DSV_PARSER_SYSERROR;
	buffer_it = buffer;
	while (*buffer_it) {
		while ((dsv_status = dsv_parser_parse_key(&buffer_it, &result_map_it->key)) == DSV_PARSER_EMPTY_LINE);
		if (dsv_status != DSV_PARSER_SUCCESS)
			return dsv_status;
		if ((dsv_status = dsv_parser_parse_value(&buffer_it, &result_map_it->value)) != DSV_PARSER_SUCCESS)
			return dsv_status;
		result_map_it++;
	}
	result_map_it->key = result_map_it->value = NULL;
	return DSV_PARSER_SUCCESS;
}
