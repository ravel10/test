import socket
import struct

from socket_tcp import server_address, checker_port, one_uint_nb_bytes,two_uint_nb_bytes,two_uint_struct_format
import mystery_calcul_fonction as gen

#Creation et paramètrage de la socket
listening_socket = socket.socket()
listening_socket.bind((server_address, checker_port))
print(f"Checker is waiting for connection")
listening_socket.listen(1)
# Attente et acceptation client

(connected_socket, client_address) = listening_socket.accept()
print(f"Checker paired succesfully")

#Reception du token client
packed_buffer = connected_socket.recv(two_uint_nb_bytes)
if len(packed_buffer) != two_uint_nb_bytes:
    raise IOError(f'Server partial read: {len(packed_buffer)} instead of {two_uint_nb_bytes}')

(secret_token,secret_key) = struct.unpack(two_uint_struct_format,packed_buffer)

if gen.key_generator(secret_token) == secret_key:
    print(f"Client as succesfully send his key : {secret_key}")
else:
    print("Client as failed")

#Fin de la communication avec le giver
try:
    connected_socket.shutdown(socket.SHUT_RDWR)
except OSError:
    connected_socket.close()
    print(f"Client has already close the socket")

connected_socket.close()
listening_socket.close()