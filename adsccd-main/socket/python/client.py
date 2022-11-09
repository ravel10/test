#!/usr/bin/env python3

import socket
import struct

from socket_conf import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# connection to the server
connected_socket = socket.create_connection((server_address, server_port))

# receive
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(val,) = struct.unpack(one_uint_struct_format, buf)
print(f"The received val was {val} .\n")

# send
connected_socket.sendall(struct.pack(one_uint_struct_format, val))


# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
