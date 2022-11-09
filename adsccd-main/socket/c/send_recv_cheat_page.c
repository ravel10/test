#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_conf.h"

int main(int argc, char **argv)
{

    ssize_t msg_size;
    uint32_t string_length;
    char *buf_string = NULL;

    /* receive the length of the string */
    msg_size = recv(connected_socket, &string_length, sizeof(string_length), 0);
    if (msg_size == -1) {
        fprintf(stderr, "server error when receiving string length: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < sizeof(string_length)) {
        fprintf(stderr, "Server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(string_length));
        goto clean;
    }

    string_length = ntohl(string_length);
    printf("Server received the string length %u .\n", string_length);

    /* allocate string */
    /* we add a final 0 to ease to print */
    buf_string = calloc(string_length + 1, sizeof(*buf_string));
    if (!buf_string) {
        fprintf(stderr, "Server error when allocating buf_string.\n");
	goto clean;
    }

    /* receive the string */
    msg_size = recv(connected_socket, buf_string, string_length, 0);
    if (msg_size == -1) {
        fprintf(stderr, "server error when receiving string: %d , %s .\n",
                        errno, strerror(errno));
        goto clean;
    }

    if (msg_size < string_length) {
        fprintf(stderr, "server only receives %ld bytes instead of %u .\n",
                        msg_size, string_length);
        goto clean;
    }

    printf("server received the string : %s .\n", buf_string);

    /*
    ======================================================================
    ======================================================================
    */

    /* send the number of byte in the string */
    string_length = htonl(string_length);
    msg_size = send(server_socket, &string_length, sizeof(string_length), 0); 
    if (msg_size == -1) {
        fprintf(stderr, "server error when sending number of bytes: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_server;
    }

    if (msg_size < sizeof(string_length)) {
        fprintf(stderr, "server as length only sends %ld bytes instead of %ld .\n",
                        msg_size, sizeof(string_length));
        goto clean_server;
    }

    string_length = ntohl(string_length);
    printf("server sent the string length.\n");

    /* send the string */
    msg_size = send(server_socket, buf_string, string_length, 0); 
    if (msg_size == -1) {
        fprintf(stderr, "server error when sending string : %d , %s .\n",
                        errno, strerror(errno));
        goto clean_server;
    }

    if (msg_size < string_length) {
        fprintf(stderr, "server as string only sends %ld bytes instead of %u .\n",
                        msg_size, string_length);
        goto clean_server;
    }

    printf("server whispered the string to server.\n");

    /* receive the treasure value */
    msg_size = recv(server_socket, &treasure_value, sizeof(treasure_value), 0);
    if (msg_size == -1) {
        fprintf(stderr, "server error when receiving treasure_value: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_server;
    }

    if (msg_size < sizeof(treasure_value)) {
        fprintf(stderr, "server only receives %ld bytes instead of %ld .\n",
                        msg_size, sizeof(treasure_value));
        goto clean_server;
    }

    treasure_value = ntohl(treasure_value);
    printf("server received the treasure_value %u .\n", treasure_value);

    /* close sockets */
    if (shutdown(server_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "server fails to shutdown the server socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(server_socket)) {
        fprintf(stderr, "server error on closing server socket: %d , %s .\n",
                        errno, strerror(errno));
	goto clean;
    }

    if (shutdown(connected_socket, SHUT_RDWR)) {
        fprintf(stderr,
                "server fails to shutdown the connected socket: "
                "%d , %s .\n", errno, strerror(errno));
    }

    if (close(connected_socket)) {
        fprintf(stderr, "server error on closing connected socket: %d , %s .\n",
                        errno, strerror(errno));
        goto clean_listening;
    }

    if (close(listening_socket)) {
        fprintf(stderr, "server error on closing listening socket: %d , %s .\n",
                        errno, strerror(errno));
	goto free_buf;
    }

    free(buf_string);
    exit(EXIT_SUCCESS);

clean_server:
    close(server_socket);
clean:
    close(connected_socket);
clean_listening:
    close(listening_socket);
free_buf:
    free(buf_string);
    exit(EXIT_FAILURE);
}



