#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_conf.h"

int main(int argc, char **argv)
{
    /* socket variables */
    struct sockaddr_in server_address;
    int connected_socket;

    /* msg variables */
    ssize_t msg_size;

    /* creating a tcp-IPv4 socket*/
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket == -1) {
        fprintf(stderr, "Client error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* connect socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(TCP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Client socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean;
    }

    if (connect(connected_socket, (struct sockaddr *)&server_address,
                sizeof(server_address))) {
        fprintf(stderr, "Client error on connecting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }


    /* send val */
    uint32_t val = 4;
    val = htonl(val);
    msg_size = send(connected_socket, &val, sizeof(val), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Client error when sending val: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(val)) {
        fprintf(stderr, "Client only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(val));
        goto clean;
    }

    val = ntohl(val);
    uint32_t score[] = {0,1,2,3};
    /* send forloop */
    for (i = 0; i < val; i++) {
        score[i] = htonl(score[i]);
        msg_size = send(connected_socket, &score[i], sizeof(score[i]), 0);
        if (msg_size == -1) {
            fprintf(stderr, "Client error when sending one val: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }
    
        if (msg_size < sizeof(score[i])) {
            fprintf(stderr, "Client only sends %ld bytes instead of %ld .\n",
                            msg_size, sizeof(score[i]));
            goto clean;
        }
    
        score[i] = ntohl(score[i]);
    }
    

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr, "Client fails to shutdown the connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Client error on closing connected socket : %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
    exit(EXIT_FAILURE);    
}
