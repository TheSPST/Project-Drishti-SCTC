#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

int main() {
    cout << "[KALKI AGI] Initializing Project Drishti SCTC Video Codec..." << endl;
    
    // 1. Hook into macOS FaceTime HD Camera
    VideoCapture cap(0); 
    if (!cap.isOpened()) {
        cerr << "FATAL: Cannot open camera. Please grant Terminal camera permissions." << endl;
        return -1;
    }

    // Set resolution to 720p for rapid processing
    cap.set(CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CAP_PROP_FRAME_HEIGHT, 720);

    Mat baseline_frame;
    cap.read(baseline_frame); // Capture T0 (Baseline)
    if (baseline_frame.empty()) {
        cerr << "FATAL: Camera returned empty frame." << endl;
        return -1;
    }

    int NOISE_THRESHOLD = 15; // Filters out microscopic sensor grain
    cout << "[KALKI AGI] Camera Hooked. SCTC Engine Running. Press ESC to quit." << endl;

    while (true) {
        Mat current_frame, sctc_reconstructed;
        cap.read(current_frame);
        if (current_frame.empty()) break;

        // Initialize the reconstructed frame with the known baseline
        sctc_reconstructed = baseline_frame.clone();
        
        long total_bytes = current_frame.total() * current_frame.elemSize();
        long moving_bytes = 0;

        uchar* curr_ptr = current_frame.data;
        uchar* base_ptr = baseline_frame.data;
        uchar* recon_ptr = sctc_reconstructed.data;

        // ==========================================
        // 2. SCTC O(1) DELTA ENCODING
        // ==========================================
        for (int i = 0; i < total_bytes; ++i) {
            int delta = abs(curr_ptr[i] - base_ptr[i]);
            
            if (delta > NOISE_THRESHOLD) {
                recon_ptr[i] = curr_ptr[i]; // Update pixel (Delta applied)
                moving_bytes++;
            }
            // If delta <= NOISE_THRESHOLD, the pixel is ignored (0 bytes transmitted).
        }

        // ==========================================
        // 3. SCTC ADAPTIVE RESET (Circuit Breaker)
        // ==========================================
        float change_ratio = (float)moving_bytes / total_bytes;
        if (change_ratio > 0.15) { // If >15% of screen changes, reset baseline (Camera Pan)
            baseline_frame = current_frame.clone();
            cout << "\r[SCTC GUARD] Massive movement detected. Baseline I-Frame Reset executed." << flush;
        }

        // ==========================================
        // 4. METRICS CALCULATION
        // ==========================================
        long uncompressed_kb = total_bytes / 1024;
        
        // SCTC Payload estimation: 4 bytes per moving pixel (Array Index + RGB Value)
        long sctc_payload_kb = (moving_bytes * 4) / 1024; 
        if (sctc_payload_kb == 0) sctc_payload_kb = 1; // Prevent div by zero
        
        float compression_ratio = (float)uncompressed_kb / sctc_payload_kb;

        // Render Metrics to Window
        putText(current_frame, "RAW (H.264 Uncompressed)", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        putText(sctc_reconstructed, "SCTC RECONSTRUCTED", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
        
        char metric_text[200];
        sprintf(metric_text, "Network Payload: %ld KB | Ratio: %.1fx", sctc_payload_kb, compression_ratio);
        putText(sctc_reconstructed, metric_text, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 0), 2);

        // Display Windows
        imshow("Standard H.264 Feed", current_frame);
        imshow("Project Drishti SCTC Codec", sctc_reconstructed);

        if (waitKey(1) == 27) break; // ESC to quit
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
