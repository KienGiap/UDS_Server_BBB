#include<Udscontrol.h>

Udscontrol::Udscontrol(ThreadSafeQueue& queue) : messageQueue(queue) {}

void Udscontrol::startService() {
    serviceRunning.store(true);
    std::thread t(&Udscontrol::handlemessage,this);
    //std::thread t1(&Udscontrol::handleservice,this);
    t.detach(); // non-blocking other operations can continue
    //t1.detach();
}

void Udscontrol::returnNegativeResponse(uint8_t sid, uint8_t nrc) {
    messageQueue.getbuffer().at(0) = 0x7F; // negative response SID
    messageQueue.getbuffer().at(1) = sid; // original request SID
    messageQueue.getbuffer().at(2) = nrc; // NRC code
     messageQueue.getbuffer().resize(3);
    messageQueue.udsHanding.store(false);
    messageQueue.cv.notify_one();
}

void Udscontrol::handlemessage()
{
    uint8_t sid;
    while(serviceRunning.load()) {
        std::unique_lock<std::mutex> lock(messageQueue.mtx);
        messageQueue.cv.wait(lock,[this] {return messageQueue.udsHanding.load();});
        sid = messageQueue.getbuffer().at(0);
        if(serviceMap.find(sid) != serviceMap.end()) {
            messageQueue.getbuffer().clear();
            std::vector<uint8_t> dummy = {50,0x48};
            messageQueue.getbuffer() = dummy;
            //messageQueue.getbuffer() = serviceMap.at(sid)->execute();
            messageQueue.udsHanding.store(false);
            messageQueue.cv.notify_one();
            // std::cout << "Supported SID" << std::endl;
        } else {
            std::cout << "Unsupported SID" << std::endl;
            returnNegativeResponse(sid, Udscontrol::REQUEST_OUT_OF_RANGE);
        }
    }
}

void Udscontrol::handleservice(){
    // TODO
    // like S3 timer, P2 timer, pending response...
}
