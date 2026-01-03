#ifndef UDSCONTROL_H
#define UDSCONTROL_H

#include <iostream>
#include<thread>
#include<atomic>
#include<ThreadSafeQueue.h>
#include<unordered_map>
#include<stdint.h>
#include<memory>
// base class for UDS services
class UdsService {
public:
    virtual ~UdsService() = default; // Added virtual destructor
    virtual std::vector<uint8_t> execute() = 0; // pure virtual function
};
// SID 0x10: Diagnostic Session Control Service
class DiagnosticSessionControl : public UdsService {
public:
    // ~DiagnosticSessionControl() {};
    std::vector<uint8_t> execute() override {
        // Implementation for Diagnostic Session Control
        // TODO
        return {};
    }
};
// SID 0x22: Read Data by Identifier Service
class ReadDataByIdentifier : public UdsService {
public:
    std::vector<uint8_t> execute() override {
        // Implementation for Diagnostic Session Control
        // TODO
        return {50,0x4C};
    }
};
// SID 0x2E: Write Data by Identifier Service
class WriteDataByIdentifier : public UdsService {
public:
    std::vector<uint8_t> execute() override {
        // Implementation for Write Data by Identifier
        // TODO
        return {};
    }
};

class Udscontrol {
public: 
    enum NRC_CODE_RESPONSE{
        GENERAL_REJECT = 0x10,
        SERVICE_NOT_SUPPORTED = 0x11,
        SUB_FUNCTION_NOT_SUPPORTED = 0x12,
        INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT = 0x13,
        BUSY_REPEAT_REQUEST = 0x21,
        CONDITIONS_NOT_CORRECT = 0x22,
        REQUEST_SEQUENCE_ERROR = 0x24,
        REQUEST_OUT_OF_RANGE = 0x31,
        SECURITY_ACCESS_DENIED = 0x33,
        UPLOAD_DOWNLOAD_NOT_ACCEPTED = 0x70,
        PENDING_RESPONSE = 0x78,
    };

    Udscontrol(ThreadSafeQueue& queue);
    ~Udscontrol() { stopService(); }
    void startService();
    void stopService() { serviceRunning.store(false); }
    void returnNegativeResponse(uint8_t sid, uint8_t nrc);
    std::unordered_map<uint16_t,std::unique_ptr<UdsService>> serviceMap;
private:

    ThreadSafeQueue& messageQueue;
    std::atomic<bool> serviceRunning{false};
    void handlemessage(); // main message handler
    void handleservice(); // back ground UDS service handler
};

#endif // UDSCONTROL_H