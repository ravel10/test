import socket
import struct

from socket_tcp import server_address, giver_port, one_uint_struct_format, one_uint_nb_bytes
import mystery_calcul_fonction as gen

#Creation et paramètrage de la socket
listening_socket = socket.socket()
listening_socket.bind((server_address, giver_port))
print(f"Giver is waiting for connection")
listening_socket.listen(1)

# Attente et acceptation client
(connected_socket, client_address) = listening_socket.accept()
print(f"Checker paired succesfully")

#Reception du token client
packed_secret_token = connected_socket.recv(one_uint_nb_bytes)
if len(packed_secret_token) != one_uint_nb_bytes:
    raise IOError(f'Server partial read: {len(packed_secret_token)} instead of {one_uint_nb_bytes}')

(secret_token,) = struct.unpack(one_uint_struct_format,packed_secret_token)

#Generation de la clé secrete
secret_key = gen.key_generator(secret_token)

connected_socket.sendall(struct.pack(one_uint_struct_format, secret_key))

try:
    connected_socket.shutdown(socket.SHUT_RDWR)
except OSError:
    connected_socket.close()
    print(f"Giver has already close the socket")

listening_socket.close()

print(f'Secret key was succesfully send')