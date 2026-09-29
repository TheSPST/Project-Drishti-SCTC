#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace sctc {

    class UDPSender {
    public:
        UDPSender(const std::string& ip, int port);
        ~UDPSender();
        void send_payload(const std::vector<uint8_t>& payload);
    private:
        int sock_;
        struct sockaddr_in dest_addr_;
        uint32_t frame_id_;
    };

    class UDPReceiver {
    public:
        UDPReceiver(int port);
        ~UDPReceiver();
        std::vector<uint8_t> receive_payload();
    private:
        int sock_;
    };

} // namespace sctc
