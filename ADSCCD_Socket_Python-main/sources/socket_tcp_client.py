import socket
import random
import struct

from socket_tcp import server_address, giver_port, checker_port, one_uint_struct_format, one_uint_nb_bytes, \
    two_uint_struct_format

#Creation de la socket client
connected_socket = socket.create_connection((server_address, giver_port))
print(f"Client paired succesfully")

# picking a random number
random.seed()
secret_token = random.randint(0, 256)

connected_socket.sendall(struct.pack(one_uint_struct_format, secret_token))

packed_secret_key = connected_socket.recv(one_uint_nb_bytes)
if len(packed_secret_key) != one_uint_nb_bytes:
    raise IOError(f'Server partial read: {len(packed_secret_key)} instead of {one_uint_nb_bytes}')

(secret_key,) = struct.unpack(one_uint_struct_format, packed_secret_key)

#Fin de la communication avec le giver
try:
    connected_socket.shutdown(socket.SHUT_RDWR)
except OSError:
    connected_socket.close()
    print(f"Giver has already close the socket")

#Creation de la socket client vers le checker
connected_socket = socket.create_connection((server_address, checker_port))

connected_socket.sendall(struct.pack(two_uint_struct_format, secret_token,secret_key))

#Fin de la communication avec le giver
connected_socket.shutdown(socket.SHUT_RDWR)
connected_socket.close()

print(f"Client has finished")