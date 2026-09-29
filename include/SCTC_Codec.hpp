#pragma once
#include <vector>
#include <cstdint>
#include <iostream>

namespace sctc {

    // Represents a single compressed pixel delta (7 bytes)
    #pragma pack(push, 1)
    struct PixelDelta {
        uint16_t idx; // Flattened 1D array index (supports up to 65535, meaning we chunk the frame)
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    #pragma pack(pop)

    class Encoder {
    public:
        Encoder(int width, int height, int noise_threshold = 15);
        
        // Encodes a raw RGB frame into a compressed byte array
        std::vector<uint8_t> encode(const uint8_t* raw_rgb);
        void force_iframe();

    private:
        int width_, height_, total_bytes_;
        int threshold_;
        std::vector<uint8_t> baseline_;
        bool require_iframe_;
    };

    class Decoder {
    public:
        Decoder(int width, int height);
        
        // Decodes the compressed byte array back into a raw RGB frame
        void decode(const std::vector<uint8_t>& compressed_payload, uint8_t* output_rgb);

    private:
        int width_, height_, total_bytes_;
        std::vector<uint8_t> reconstructed_;
    };

} // namespace sctc
