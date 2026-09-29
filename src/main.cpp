#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "../include/SCTC_Codec.hpp"

using namespace cv;
using namespace std;

int main() {
    cout << "[KALKI AGI] Senior Engineering Protocol: OOP Codec Instantiated." << endl;
    
    VideoCapture cap(0); 
    if (!cap.isOpened()) {
        cerr << "FATAL: Cannot open camera." << endl;
        return -1;
    }

    int width = 640; 
    int height = 480;
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);

    sctc::Encoder encoder(width, height, 15);
    sctc::Decoder decoder(width, height);

    Mat current_frame, reconstructed_frame(height, width, CV_8UC3);

    // ==========================================
    // SCTC TELEMETRY LOGGER
    // ==========================================
    ofstream logfile("drishti_telemetry.csv");
    logfile << "Frame_Number,Raw_Size_KB,SCTC_Size_KB,Compression_Ratio,Is_I_Frame_Reset\n";
    int frame_count = 0;
    cout << "[KALKI AGI] Telemetry Logging initialized -> drishti_telemetry.csv" << endl;

    while (true) {
        cap.read(current_frame);
        if (current_frame.empty()) break;

        // 1. ENCODE (Simulates Node A)
        std::vector<uint8_t> compressed_payload = encoder.encode(current_frame.data);
        
        // Detect if this frame triggered the Adaptive Camera Pan Reset (I-Frame)
        bool is_iframe = (!compressed_payload.empty() && compressed_payload[0] == 0x01);
        
        // 2. DECODE (Simulates Node B)
        decoder.decode(compressed_payload, reconstructed_frame.data);

        // 3. METRICS & LOGGING
        long raw_kb = (width * height * 3) / 1024;
        long sctc_kb = compressed_payload.size() / 1024;
        if (sctc_kb == 0) sctc_kb = 1;
        
        float ratio = (float)raw_kb / sctc_kb;

        // Write live data to the CSV file
        logfile << frame_count << "," << raw_kb << "," << sctc_kb << "," << ratio << "," << (is_iframe ? "YES" : "NO") << "\n";
        frame_count++;

        // Render to Screen
        putText(current_frame, "RAW Uncompressed", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        putText(reconstructed_frame, "SCTC Decoder", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
        
        char metric_text[100];
        sprintf(metric_text, "Network Payload: %ld KB | Ratio: %.1fx", sctc_kb, ratio);
        putText(reconstructed_frame, metric_text, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 0), 2);

        imshow("Node A (Sender)", current_frame);
        imshow("Node B (Receiver)", reconstructed_frame);

        if (waitKey(1) == 27) break; // Press ESC to quit
    }

    cap.release();
    logfile.close();
    destroyAllWindows();
    cout << "[KALKI AGI] Session closed. Empirical data saved to: drishti_telemetry.csv" << endl;
    return 0;
}
