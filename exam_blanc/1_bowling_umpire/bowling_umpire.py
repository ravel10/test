#!/usr/bin/env python3

import socket
import random
import struct

from socket_conf import server_address, server_port, one_uint_struct_format, one_uint_nb_bytes

# socket creation
# python 3.8 : listening_socket = socket.create_server(('localhost', 60000))
listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))
listening_socket.listen(1)

# accepting client
(connected_socket, client_address) = listening_socket.accept()

#receive random number
buf = connected_socket.recv(one_uint_nb_bytes)

(nb_joueurs,) = struct.unpack(one_uint_struct_format, buf)
score=[]

for i in range(nb_joueurs): 
	buf = connected_socket.recv(one_uint_nb_bytes)
	(temp, ) = struct.unpack(one_uint_struct_format, buf)
	score.append(temp)



# close connection
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()
listening_socket.close()

# print result
print(f'Nb joueurs: {nb_joueurs}')

for i in range(nb_joueurs):
	print(f'{i}: {score[i]}')

