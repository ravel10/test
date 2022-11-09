import struct

one_uint_struct_format = '!I'
one_uint_nb_bytes = struct.calcsize(one_uint_struct_format)

two_uint_struct_format = '!2I'
two_uint_nb_bytes = struct.calcsize(two_uint_struct_format)

server_address = 'localhost'
giver_port = 60000
checker_port = 60001
