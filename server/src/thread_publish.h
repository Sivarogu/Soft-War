#pragma once
#include <czmq.h>

void *thread_publish_run();
int thread_publish_start();
int thread_publish_start_cycles(zsock_t *socket);
