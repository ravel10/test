import struct

server_address = 'localhost'
server_port = 60000

struc_encode_nbBytes_format = '!I'

struc_nb_bytes = struct.calcsize(struc_encode_nbBytes_format)