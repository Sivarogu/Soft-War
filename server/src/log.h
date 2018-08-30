#pragma once

typedef enum {
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR
} t_log_level;

int log_init();
int log_log(t_log_level level, const char *str);
int log_debug(const char *str, ...);
int log_info(const char *str, ...);
int log_warn(const char *str, ...);
int log_error(const char *str, ...);
const char *log_level_name(t_log_level level);
