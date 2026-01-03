#include <iostream>
#include <TcpServer.h>
#include <ThreadSafeQueue.h>
#include <Udscontrol.h>

int main() {
    std::cout << "TCP Server Application Started" << std::endl;
    
    ThreadSafeQueue queue(1024);
    TcpServer server(12345,queue); // Create a TCP server on port 8080
    server.start(); // Start the server

    Udscontrol udscontrol(queue);
    udscontrol.serviceMap.emplace(0x10, std::make_unique<DiagnosticSessionControl>());
    udscontrol.startService();

    while(1) {
        std::this_thread::sleep_for(std::chrono::seconds(1000));
    }; // Keep the main thread alive
    return 0;
}