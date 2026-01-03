#include<TcpServer.h>
#include<cstdlib>

TcpServer::TcpServer(int p, ThreadSafeQueue& queue) : port(p), messageQueue(queue) {}

void TcpServer::start() {
    is_running.store(true);
    std::thread t(&TcpServer::handlemessage,this);
    // t.join();
    t.detach(); // non-blocking other operations can continue
}

void TcpServer::handlemessage() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Ipv4 TCP socket
    if (serverSocket < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // Ipv4
    serverAddress.sin_port = htons(port); // port
    serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections from any address
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5); // listen for incoming connections
    // inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
    while(is_running.load()) {
        // try lock mutex continuously
        std::unique_lock<std::mutex> lock(messageQueue.mtx);
        // // wait until previous UDS ID is done
        messageQueue.cv.wait(lock,[this] {return !messageQueue.udsHanding.load();});
        // accept incoming connection
        int clientSocket = accept(serverSocket,NULL,NULL);
        // resize vector, because recv function needs a buffer with fixed size
        messageQueue.getbuffer().resize(BUFFER_SIZE);
        int len = recv(clientSocket, messageQueue.getbuff(), BUFFER_SIZE, 0);

        if (len > 0) {  // message received successfully
            inputDataLog(len);
            // Send request to UDS control via thread-safe queue
            messageQueue.udsHanding.store(true); // set flag to indicate UDS message handling
            messageQueue.cv.notify_all(); // notify UDS thread
            messageQueue.cv.wait(lock,[this] {return !messageQueue.udsHanding.load();});
            // response back to client
            std::cout << "Sending response back to client" << std::endl;
            send_response(clientSocket);
        }
        
        close(clientSocket);
    }
    std::cout << "Close socket " << std::endl;
    close(serverSocket);
}

void TcpServer::inputDataLog(int len)
{
    std::cout << "Message received: ";
    for (int i = 0; i < len; i++)
    {
        std::cout << std::hex << static_cast<int>(messageQueue.getbuff()[i]) << " ";
    }
    std::cout << std::endl;
}

void TcpServer::stop() {
    is_running = false;
}

void TcpServer::send_response(const int &clientSocket) {
    std::vector<uint8_t> dummy_response = {0x50, 0x01, 0x02, 0x03}; // Example response data
    // send(clientSocket, dummy_response.data(), dummy_response.size(), 0);
    send(clientSocket, messageQueue.getbuff(), messageQueue.buffersize(), 0);
}
