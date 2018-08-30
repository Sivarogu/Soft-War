#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "tty_color.h"
#include "global_config.h"

static FILE *log_file = NULL;

const char *log_level_name(t_log_level level) {
	switch (level) {
		case LOG_LEVEL_DEBUG: return "DEBUG";
		case LOG_LEVEL_INFO: return "INFO";
		case LOG_LEVEL_WARN: return "WARN";
		case LOG_LEVEL_ERROR: return "ERROR";
	}
	return NULL;
}

int log_init() {
	BIND_MZERO((log_file = fopen(global_config.log_file, "a")), "could not open logs file: %s", global_config.log_file);
	return 0;
}

int log_log(t_log_level level, const char *str) {
	const char *color;
	char *date;
	char *datetime;

	switch (level) {
		case LOG_LEVEL_DEBUG:
			color = TTY_COLOR_FG_BLUE;
			break;
		case LOG_LEVEL_INFO:
			color = TTY_COLOR_FG_GREEN;
			break;
		case LOG_LEVEL_WARN:
			color = TTY_COLOR_FG_LIGHT_YELLOW;
			break;
		case LOG_LEVEL_ERROR:
			color = TTY_COLOR_FG_LIGHT_RED;
			break;
	}
	BIND_ZERO(date = buildstr_current_time());
	BIND_ZERO(datetime = buildstr_current_datetime());
	if (level >= global_config.log_level) {
		if (log_file) {
			BIND_NEG(fprintf(log_file, "%s %-5s %s\n", datetime, log_level_name(level), str));
			fflush(log_file);
		}
		BIND_NEG(printf(TTY_COLOR_FG_DARK_GREY "%s" TTY_COLOR_RESET " %s%-5s" TTY_COLOR_RESET " %s\n", date, color, log_level_name(level), str));
	}
	free(date);
	free(datetime);
	return 0;
}

int log_debug(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_NEG(vasprintf(&formated_str, fmt, args));
	BIND_NEG(log_log(LOG_LEVEL_DEBUG, formated_str));
	va_end(args);
	free(formated_str);
	return 0;
}


int log_info(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_NEG(vasprintf(&formated_str, fmt, args));
	BIND_NEG(log_log(LOG_LEVEL_INFO, formated_str));
	va_end(args);
	free(formated_str);
	return 0;
}

int log_warn(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_NEG(vasprintf(&formated_str, fmt, args));
	BIND_NEG(log_log(LOG_LEVEL_WARN, formated_str));
	va_end(args);
	free(formated_str);
	return 0;
}

int log_error(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_NEG(vasprintf(&formated_str, fmt, args));
	BIND_NEG(log_log(LOG_LEVEL_ERROR, formated_str));
	va_end(args);
	free(formated_str);
	return 0;
}
