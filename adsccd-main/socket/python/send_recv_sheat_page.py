

#f = open(input_file, 'rb')
buf_size = 1024
with open(input_file, 'rb') as f:
    buf = f.read(buf_size)
    while len(buf):
        connected_socket.sendall(buf)
        buf = f.read(buf_size)


with open(output_file, 'wb') as f:
    buf = connecetd_socket.recv(buf_size)
    while buf:
        if f.write(buf) != len(buf):
            raise IOError('Server partial write')

        buf = connected_socket.recv(buf_size)