import socket
import time
#server information
server_hostname = '192.168.0.105'
server_port = 12123

#create clientSocket
#AF_INET means it uses IP as the network layer service
#SOCK_DGRAM means that it uses UDP as the transport layer service
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while(True):
    #Application Logic
    message = 'Small Step For Man, Giant Leap For Mankind'

    #send the message to the server
    clientSocket.sendto(message.encode('utf8'), (server_hostname, server_port))

    #wait for? recieving message
    response, serverAddress = clientSocket.recvfrom(2048)

    print("The response is", response)
    print(serverAddress)
    time.sleep(5)
    
