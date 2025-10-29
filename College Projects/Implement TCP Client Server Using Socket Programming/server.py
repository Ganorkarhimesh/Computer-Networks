import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('127.0.0.1', 8080))
server_socket.listen(1)

print("Server is listening on port 8080...")

conn, addr = server_socket.accept()
print(f"Connection established with {addr}")

data = conn.recv(1024).decode()
print("Client:", data)

conn.send("Message received successfully!".encode())

conn.close()
server_socket.close()
