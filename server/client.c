#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pour avoir un client de test (ex: envoyer ping|null)
// gcc client.c -o ./dist/softwar-client -lczmq -lzmq

void clean(const char *buffer, FILE *fp)
{
    int c;
    char *p = strchr(buffer, '\n');

    if (p != NULL)
        *p = 0;
    else
    {
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

int main()
{
    printf("Connecting to server...\n");
    char msg[20] = "";
    zsock_t *req = zsock_new(ZMQ_REQ);
    zsock_connect(req, "tcp://localhost:8765");
    int request_nbr;
    do
    {
        fgets(msg, sizeof(msg), stdin);
        clean(msg, stdin);
        zstr_sendf(req, "%s", msg);

        char *message = zstr_recv(req);
        printf("Received : %s\n", message);
        zstr_free(&message);
    } while (strcmp(msg, "exit"));

    zsock_destroy(&req);
    return 0;
}
