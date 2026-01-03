#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include<condition_variable>
#include<vector>

class ThreadSafeQueue { 
public:
    ThreadSafeQueue(int size) : buffer_(size) {} // default buffer size 1024 bytes
    void send_request(const std::vector<uint8_t>& msg, unsigned int len);
    std::vector<int> receive_message();

    int buffersize();
    uint8_t *getbuff() { return buffer_.data(); }
    std::vector<uint8_t> &getbuffer() { return buffer_; }
    std::mutex mtx; // sync rx tx thread
    std::atomic<bool> udsHanding{false}; // flag to indicate UDS message is handling/not
    std::condition_variable cv; // check status
private:
    std::vector<uint8_t> buffer_;
};

#endif