#include "../include/UDP_Transport.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <algorithm>

namespace sctc {

    const size_t CHUNK_SIZE = 60000; // Keep safely under 65KB UDP limit

    // ==========================================
    // UDP SENDER (Chunking Protocol)
    // ==========================================
    UDPSender::UDPSender(const std::string& ip, int port) : frame_id_(0) {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&dest_addr_, 0, sizeof(dest_addr_));
        dest_addr_.sin_family = AF_INET;
        dest_addr_.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dest_addr_.sin_addr);
    }

    UDPSender::~UDPSender() { close(sock_); }

    void UDPSender::send_payload(const std::vector<uint8_t>& payload) {
        frame_id_++;
        size_t offset = 0;
        uint16_t total_chunks = (payload.size() / CHUNK_SIZE) + 1;
        uint16_t chunk_id = 0;

        while (offset < payload.size()) {
            size_t current_chunk_size = std::min(CHUNK_SIZE, payload.size() - offset);
            std::vector<uint8_t> packet(8 + current_chunk_size);
            
            // 8-Byte Custom SCTC Network Header
            memcpy(packet.data(), &frame_id_, 4);
            memcpy(packet.data() + 4, &total_chunks, 2);
            memcpy(packet.data() + 6, &chunk_id, 2);
            
            // Payload
            memcpy(packet.data() + 8, payload.data() + offset, current_chunk_size);
            
            sendto(sock_, packet.data(), packet.size(), 0, (struct sockaddr*)&dest_addr_, sizeof(dest_addr_));
            
            offset += current_chunk_size;
            chunk_id++;
        }
    }

    // ==========================================
    // UDP RECEIVER (Reassembly Protocol)
    // ==========================================
    UDPReceiver::UDPReceiver(int port) {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);
        bind(sock_, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    UDPReceiver::~UDPReceiver() { close(sock_); }

    std::vector<uint8_t> UDPReceiver::receive_payload() {
        uint32_t current_frame_id = 0;
        uint16_t expected_chunks = 0;
        std::vector<std::vector<uint8_t>> chunks(200); 
        int chunks_received = 0;

        while (true) {
            uint8_t buffer[65000];
            int len = recvfrom(sock_, buffer, 65000, 0, NULL, NULL);
            if (len < 8) continue;

            uint32_t frame_id;
            uint16_t total_chunks;
            uint16_t chunk_id;
            memcpy(&frame_id, buffer, 4);
            memcpy(&total_chunks, buffer + 4, 2);
            memcpy(&chunk_id, buffer + 6, 2);

            // If we receive a packet from a NEW frame, discard old incomplete frames
            if (frame_id != current_frame_id) {
                current_frame_id = frame_id;
                expected_chunks = total_chunks;
                chunks_received = 0;
            }

            if (chunk_id < 200) {
                chunks[chunk_id] = std::vector<uint8_t>(buffer + 8, buffer + len);
                chunks_received++;
            }

            // Reassemble when all chunks arrive
            if (chunks_received == expected_chunks) {
                std::vector<uint8_t> full_payload;
                for (int i = 0; i < expected_chunks; i++) {
                    full_payload.insert(full_payload.end(), chunks[i].begin(), chunks[i].end());
                }
                return full_payload;
            }
        }
    }
} // namespace sctc
