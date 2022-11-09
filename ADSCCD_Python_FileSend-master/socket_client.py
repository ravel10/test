import socket
import struct

from socket_tcp_settings import server_address,server_port

#Creation de la socket client
connected_socket = socket.create_connection((server_address, server_port))
print(f"Client paired succesfully")

input_file = open("InputFolder/input.txt", "r")
curr = input_file.readline()

if curr == "" :
    connected_socket.sendall(struct.pack('!I', 0))

#Lecture et envoi du fichier
while curr != "" :

    input_bytes = bytes(curr,'utf-8')
    connected_socket.sendall(struct.pack('!I', len(input_bytes)))
    connected_socket.sendall(struct.pack('!{}s'.format(len(input_bytes)), input_bytes))
    curr = input_file.readline()

input_file.close()

#Les traitements du serveur en lecture étant plus long, je laisse ce dernier shutdown en lecture puis fermer la socket
connected_socket.shutdown(socket.SHUT_RDWR)
print(f"Client has finished")