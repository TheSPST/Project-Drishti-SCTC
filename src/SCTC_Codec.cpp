#include "../include/SCTC_Codec.hpp"
#include <cmath>
#include <cstring>

namespace sctc {

// ==========================================
// ENCODER IMPLEMENTATION
// ==========================================
Encoder::Encoder(int width, int height, int noise_threshold) 
    : width_(width), height_(height), threshold_(noise_threshold), require_iframe_(true) {
    total_bytes_ = width * height * 3;
    baseline_.resize(total_bytes_, 0);
}

void Encoder::force_iframe() { require_iframe_ = true; }

std::vector<uint8_t> Encoder::encode(const uint8_t* raw_rgb) {
    std::vector<uint8_t> payload;
    
    if (require_iframe_) {
        // [Header: 0x01 = I-Frame]
        payload.push_back(0x01);
        payload.insert(payload.end(), raw_rgb, raw_rgb + total_bytes_);
        std::memcpy(baseline_.data(), raw_rgb, total_bytes_);
        require_iframe_ = false;
        return payload;
    }

    // [Header: 0x02 = P-Frame (Delta)]
    payload.push_back(0x02);
    
    int moving_pixels = 0;
    
    // O(1) Array Subtraction
    for (int i = 0; i < total_bytes_; i += 3) {
        int dr = std::abs(raw_rgb[i] - baseline_[i]);
        int dg = std::abs(raw_rgb[i+1] - baseline_[i+1]);
        int db = std::abs(raw_rgb[i+2] - baseline_[i+2]);
        
        // Threshold Quantization (Noise Gate)
        if (dr > threshold_ || dg > threshold_ || db > threshold_) {
            // Pack the Delta
            uint32_t pixel_idx = i / 3;
            // Note: For 1080p, pixel_idx > 65535, so we use 32-bit index in real prod.
            // For prototype, we pack standard 32-bit index.
            uint8_t idx_bytes[4];
            std::memcpy(idx_bytes, &pixel_idx, 4);
            
            payload.insert(payload.end(), idx_bytes, idx_bytes + 4);
            payload.push_back(raw_rgb[i]);
            payload.push_back(raw_rgb[i+1]);
            payload.push_back(raw_rgb[i+2]);
            
            // Update Baseline
            baseline_[i] = raw_rgb[i];
            baseline_[i+1] = raw_rgb[i+1];
            baseline_[i+2] = raw_rgb[i+2];
            moving_pixels++;
        }
    }

    // Adaptive Circuit Breaker (Camera Pan Detection)
    float changed_ratio = (float)moving_pixels / (width_ * height_);
    if (changed_ratio > 0.15) {
        force_iframe(); // The next frame will be a full refresh
    }

    return payload;
}

// ==========================================
// DECODER IMPLEMENTATION
// ==========================================
Decoder::Decoder(int width, int height) : width_(width), height_(height) {
    total_bytes_ = width * height * 3;
    reconstructed_.resize(total_bytes_, 0);
}

void Decoder::decode(const std::vector<uint8_t>& payload, uint8_t* output_rgb) {
    if (payload.empty()) return;
    
    uint8_t header = payload[0];
    
    if (header == 0x01) { // I-Frame
        std::memcpy(reconstructed_.data(), payload.data() + 1, total_bytes_);
    } 
    else if (header == 0x02) { // P-Frame (Delta)
        const uint8_t* data = payload.data() + 1;
        size_t size = payload.size() - 1;
        
        for (size_t i = 0; i < size; i += 7) { // 4 byte index + 3 byte RGB
            uint32_t pixel_idx;
            std::memcpy(&pixel_idx, data + i, 4);
            
            int array_idx = pixel_idx * 3;
            if (array_idx + 2 < total_bytes_) {
                reconstructed_[array_idx] = data[i+4];
                reconstructed_[array_idx+1] = data[i+5];
                reconstructed_[array_idx+2] = data[i+6];
            }
        }
    }
    
    std::memcpy(output_rgb, reconstructed_.data(), total_bytes_);
}

} // namespace sctc
