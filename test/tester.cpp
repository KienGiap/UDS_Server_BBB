// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    // sending data
    const char message[] = {0x22, 0x4C, 0x20}; // Example message data
    send(clientSocket, message, strlen(message), 0);
    std::cout << "Message sent: ";
    for (size_t i = 0; i < strlen(message); i++) {
        std::cout << std::hex << static_cast<int>(message[i]) << " ";
    }
    std::cout << std::endl;

    uint8_t buffer[1024];
    int len = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (len > 0) {
        std::cout << "Message received: ";
        for (int i = 0; i < len; i++) {
            std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Failed to receive message from server" << std::endl;
    }

    // closing socket
    close(clientSocket);

    return 0;
}