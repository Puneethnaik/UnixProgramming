import socket

serverPort = 12123

serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.bind(('localhost', serverPort))


requestBody, clientAddress = serverSocket.recvfrom(2048)

responseBody = requestBody.upper()

serverSocket.sendto(responseBody, clientAddress)

