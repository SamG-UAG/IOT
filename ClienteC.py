import socket

# Create a socket object
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Get local machine name
host = socket.gethostname()
port = 9999

# Connect to server
clientsocket.connect((host, port))

# Send a thank you message to the client
message = "Thank you for connecting" + "\r\n"
clientsocket.sendall(message.encode('utf-8'))

# Receive data from the server
data = clientsocket.recv(1024)
clientsocket.close()

print(repr(data))
