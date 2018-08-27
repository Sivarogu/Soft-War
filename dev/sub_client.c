#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "../server/src/utils.h"
#include "../server/src/tty_color.h"
#include "../server/src/log.h"

char *buildstr(const char *fmt, ...) {
	va_list args;
	char *formated_str;

	va_start(args, fmt);
	BIND_RNEG(vasprintf(&formated_str, fmt, args), NULL);
	va_end(args);
	return formated_str;
}

char *buildstr_current_time() {
	time_t current_time;
	struct tm * time_info;

	time(&current_time);
	time_info = localtime(&current_time);
	return buildstr("%02d:%02d:%02d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

const char *log_level_name(t_log_level level) {
	switch (level) {
		case LOG_LEVEL_DEBUG: return "DEBUG";
		case LOG_LEVEL_INFO: return "INFO";
		case LOG_LEVEL_WARN: return "WARN";
		case LOG_LEVEL_ERROR: return "ERROR";
	}
}

int log_log(t_log_level level, const char *str) {
	const char *color;
	const char *date;

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
	BIND_NEG(printf(TTY_COLOR_FG_DARK_GREY "%s" TTY_COLOR_RESET " %s%-5s" TTY_COLOR_RESET " %s\n", date, color, log_level_name(level), str));
	return 0;
}

int main()
{
    char *debug_message;
    log_log(LOG_LEVEL_INFO, "connecting to server...");
    zsock_t *subscriber = zsock_new(ZMQ_SUB);
    if (zsock_connect(subscriber, "tcp://localhost:4243") != 0)
    {
        log_log(LOG_LEVEL_ERROR, "could not connect subscribe socket");
        return EXIT_FAILURE;
    }
    log_log(LOG_LEVEL_INFO, "succesfully connected!");

    zsock_set_subscribe(subscriber, "");
    sleep(1);
    while (!zsys_interrupted) {
        char *message = zstr_recv(subscriber);
        BIND_ZERO(debug_message = buildstr("frame: %s", message));
        log_log(LOG_LEVEL_DEBUG, debug_message);
        zstr_free(&message);
        free(debug_message);
    }
    zmq_close(subscriber);
    zsock_destroy(&subscriber);
    return 0;
}
