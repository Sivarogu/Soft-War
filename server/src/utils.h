#pragma once
#include <stddef.h>
#include "log.h"

#define BIND_TRUE(__expr) {if (__expr) return -1;}
#define BIND_MTRUE(__expr, ...) {if (__expr) {log_error(__VA_ARGS__); return -1;}}
#define BIND_NEG(__expr) {if ((__expr) < 0) return -1;}
#define BIND_RNEG(__expr, __ret) {if ((__expr) < 0) return __ret;}
#define BIND_MNEG(__expr, ...) {if ((__expr) < 0) {log_error(__VA_ARGS__); return -1;}}
#define BIND_ZERO(__expr) {if ((__expr) == NULL) return -1;}
#define BIND0_ZERO(__expr) {if ((__expr) == NULL) return NULL;}
#define BIND_RZERO(__expr, __ret) {if ((__expr) == NULL) return __ret;}
#define BIND_MZERO(__expr, ...) {if ((__expr) == NULL) {log_error(__VA_ARGS__); return -1;}}

char *buildstr(const char *fmt, ...);
char *buildstr_current_date();
char *buildstr_current_time();
char *buildstr_current_datetime();
