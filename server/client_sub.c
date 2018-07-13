#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pour avoir un client de test (ex: envoyer ping|null)
// gcc client_sub.c -o ./softwar-sub-client -lczmq -lzmq

int main()
{
    printf("Connecting to server...");
    zsock_t *subscriber = zsock_new(ZMQ_SUB);
    if (zsock_connect(subscriber, "tcp://localhost:4243") != 0)
    {
        printf("FAIL\n");
        return EXIT_FAILURE;
    }
    else
    {
        printf("SUCCESS...so far...\n");
    }
    // zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    zsock_set_subscribe(subscriber, "");
    sleep(1);
    while (!zsys_interrupted)
    {
        char *message = zstr_recv(subscriber);
        printf("Test : %s\n", message);
        zstr_free(&message);
    }
    zmq_close(subscriber);
    zsock_destroy(&subscriber);
    return 0;
}
