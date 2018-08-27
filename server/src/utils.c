#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char *buildstr(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_RNEG(vasprintf(&formated_str, fmt, args), NULL);
	va_end(args);
	return formated_str;
}

char *buildstr_current_date() {
	time_t current_time;
	struct tm * time_info;

	time(&current_time);
	time_info = localtime(&current_time);
	return buildstr("%04d-%02d-%02d", time_info->tm_year + 1900, time_info->tm_mon, time_info->tm_mday);
}

char *buildstr_current_time() {
	time_t current_time;
	struct tm * time_info;

	time(&current_time);
	time_info = localtime(&current_time);
	return buildstr("%02d:%02d:%02d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

char *buildstr_current_datetime() {
	char *current_date;
	char *current_time;
	char *current_datetime;

	BIND_RZERO((current_date = buildstr_current_date()), NULL);
	BIND_RZERO((current_time = buildstr_current_time()), NULL);
	BIND_RZERO((current_datetime = buildstr("%s %s", current_date, current_time)), NULL);
	free(current_date);
	free(current_time);
	return current_datetime;
}
