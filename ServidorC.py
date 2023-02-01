import socket

# Create a socket object
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Get local machine name
host = socket.gethostname()
port = 9999

# Bind the socket to a public host, and a port
serversocket.bind((host, port))

# Become a server socket
serversocket.listen(5)

while True:
    # Establish a connection
    clientsocket, addr = serversocket.accept()

    print("Got a connection from %s" % str(addr))
    msg = "Hello World" + "\r\n"
    clientsocket.sendall(msg.encode('utf-8'))
    while True:
        try:
            data = clientsocket.recv(1024)
        except ConnectionAbortedError as e:
            print("Connection closed by client:", e)
            break
        data = clientsocket.recv(1024)
        if not data:
            break
        clientsocket.sendall(data)
    clientsocket.close()

