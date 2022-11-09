#!/usr/bin/env python3

import socket
import random
import struct

from socket_conf import whisperer_destination_ipv4_addr, whisperer_connection_port, vault_ipv4_addr, vault_tcp_port, one_uint_struct_format, one_uint_nb_bytes


# socket creation
# python 3.8 : listening_socket = socket.create_server(('localhost', 60000))
listening_socket = socket.socket()
listening_socket.bind((whisperer_destination_ipv4_addr, whisperer_connection_port))
listening_socket.listen(1)

# accepting client
(connected_socket, client_address) = listening_socket.accept()

#receive mdp
buf = connected_socket.recv(one_uint_nb_bytes)

(longueur,) = struct.unpack(one_uint_struct_format, buf)
mdp=[]

for i in range(longueur): 
	buf = connected_socket.recv(sizeof(char))
	(temp, ) = struct.unpack(sizeof("B", buf)
	mdp.append(temp)




# connection to the server
connected_socket2 = socket.create_connection((vault_ipv4_addr, vault_tcp_port))


connected_socket2.sendall(struct.pack(one_uint_struct_format, longueur))

for i in range(longeur):
	temp = mdp[i]
	connected_socket2.sendall(struct.pack(one_uint_struct_format, temp))


#receive prix global from the server
buf = connected_socket.recv(one_uint_nb_bytes)
if len(buf) != one_uint_nb_bytes:
    raise IOError(f'Client partial read: {len(buf)} instead of {one_uint_nb_bytes}')

(valeurTresor,) = struct.unpack(one_uint_struct_format, buf)


# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

# print result
for i in range(nb_joueurs):
	print(f'{mdp[i]}')
print(f'{valeurTresor}')
