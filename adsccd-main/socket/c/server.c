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
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int listening_socket;
    int connected_socket;

    /*msg variables*/
    ssize_t msg_size;
    int fakebuf;

    /* creating a tcp-IPv4 socket*/
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        fprintf(stderr, "Server error on creating the socket : %d, %s.\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* bind socket */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(TCP_PORT);
    if (!inet_aton(SERVER_IPV4_ADDR, &server_address.sin_addr)) {
        fprintf(stderr, "Server socket adress is not valid : %s \n",
                        SERVER_IPV4_ADDR);
        goto clean_listening;
    }
    if (bind(listening_socket, (struct sockaddr *)&server_address,
             sizeof(server_address))) {
        fprintf(stderr, "Server error on binding : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    /* listen */
    if (listen(listening_socket, 1)) {
        fprintf(stderr, "Server unable to listen %d max client : %d , %s .\n",
                        1, errno, strerror(errno));
        goto clean_listening;
    }
    
    /* accept one connection */
    connected_socket = accept(listening_socket,
                              (struct sockaddr *)&client_address,
                              &client_address_len);
    if (connected_socket == -1) {
        fprintf(stderr, "Server error on accepting socket : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (client_address_len > sizeof(client_address)) {
        fprintf(stderr, "Server error on accepting a client address size too "
                        "large : %d accepted, instead of %ld .\n",
                        client_address_len, sizeof(client_address));
        goto clean;
    }

    
    /* receive uint32_t */
    uint32_t val;
    msg_size = recv(connected_socket, &val, sizeof(val), 0);
    if (msg_size == -1) {
        fprintf(stderr, "Server error when receiving number of player: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(val)) {
        fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(val));
        goto clean;
    }

    val = ntohl(val);


    /* receives for loop */
    unsigned int i;
    unsigned int n=val;
    for (i = 0; i < n; i++) {
	    uint32_t val;

        msg_size = recv(connected_socket, &val, sizeof(val), 0);
        if (msg_size == -1) {
            fprintf(stderr, "Server error when receiving msg: %d , %s .\n",
                            errno, strerror(errno));
            goto clean;
        }
    
        if (msg_size < sizeof(val)) {
            fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                            msg_size, sizeof(score));
            goto clean;
        }
    
        val = ntohl(val);

        /*LOGIC*/
    }

    /* close the sockets */
    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "Server fails to shutdown the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "Server error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "Server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* print result */
    printf("RES:...\n");
    exit(EXIT_SUCCESS);

clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
    exit(EXIT_FAILURE);    
}
