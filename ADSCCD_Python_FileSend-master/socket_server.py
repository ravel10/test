import socket
import struct
import os

from socket_tcp_settings import server_address, server_port, struc_nb_bytes

#Creation et paramètrage de la socket
listening_socket = socket.socket()
listening_socket.bind((server_address, server_port))

# Attente et acceptation client
print(f"Checker is waiting for connection")
listening_socket.listen(1)
(connected_socket, client_address) = listening_socket.accept()
print(f"Checker paired succesfully")

packed_buffer = connected_socket.recv(struc_nb_bytes)

if len(packed_buffer) != struc_nb_bytes:
    raise IOError(f'Server partial read: {len(packed_buffer)} instead of {struc_nb_bytes}')

(bytes_len,) = struct.unpack("!I",packed_buffer)
print(f"Going to receive {bytes_len} bytes")


#Création du fichier
if bytes_len != 0 :

    try:
        os.remove("./OutputFolder/output.txt")
        print(f"File removed")
    except OSError:
        print(f"File can't be removed")

    fichier = open("./OutputFolder/output1.txt", "w")

    while bytes_len != 0 :

        string_lenth = struct.calcsize('!{}s'.format(bytes_len))
        packed_string = connected_socket.recv(string_lenth)

        if len(packed_string) != string_lenth:
            raise IOError(f'Server partial read: {len(packed_string)} instead of {string_lenth}')

        fichier.write(packed_string.decode("utf-8"))

        packed_buffer = connected_socket.recv(struc_nb_bytes)

        if len(packed_buffer) != struc_nb_bytes & len(packed_buffer) != 0:
            raise IOError(f'Server partial read: {len(packed_buffer)} instead of {struc_nb_bytes}')

        if len(packed_buffer) == 0:
            break

        (bytes_len,) = struct.unpack("!I", packed_buffer)
        print(f"Going to receive {bytes_len} bytes")

fichier.close()

connected_socket.close()

listening_socket.close()