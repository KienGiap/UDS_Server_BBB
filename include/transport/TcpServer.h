#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <thread>
#include<stdint.h>
#include <arpa/inet.h>
#include<ThreadSafeQueue.h>
#include<vector>

#define BUFFER_SIZE 1025

class TcpServer {
public:
    TcpServer(int p, ThreadSafeQueue& queue);
    ~TcpServer() { stop(); }
    void start();
    void handlemessage();
    void inputDataLog(int len);
    void stop();
    void send_response(const int &clientSocket);
private:
    int port;
    ThreadSafeQueue& messageQueue;
    std::atomic<bool> is_running{false};
};

#endif // TCPSERVER_H