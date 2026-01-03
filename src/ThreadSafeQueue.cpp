#include<ThreadSafeQueue.h>

void ThreadSafeQueue::send_request(const std::vector<uint8_t> &msg, unsigned int len)
{
    std::lock_guard<std::mutex> lock(mtx);
}

int ThreadSafeQueue::buffersize()
{
    return buffer_.size();
}
