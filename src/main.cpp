#include <opencv2/opencv.hpp>
#include <iostream>
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

    int width = 640; // Downscaled for rapid network prototyping
    int height = 480;
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);

    sctc::Encoder encoder(width, height, 15);
    sctc::Decoder decoder(width, height);

    Mat current_frame, reconstructed_frame(height, width, CV_8UC3);

    while (true) {
        cap.read(current_frame);
        if (current_frame.empty()) break;

        // 1. ENCODE (Simulates Node A)
        std::vector<uint8_t> compressed_payload = encoder.encode(current_frame.data);
        
        // 2. DECODE (Simulates Node B)
        decoder.decode(compressed_payload, reconstructed_frame.data);

        // 3. METRICS
        long raw_kb = (width * height * 3) / 1024;
        long sctc_kb = compressed_payload.size() / 1024;
        if (sctc_kb == 0) sctc_kb = 1;

        putText(current_frame, "RAW Uncompressed", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
        putText(reconstructed_frame, "SCTC Decoder", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
        
        char metric_text[100];
        sprintf(metric_text, "Network Payload: %ld KB | Ratio: %.1fx", sctc_kb, (float)raw_kb / sctc_kb);
        putText(reconstructed_frame, metric_text, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 0), 2);

        imshow("Node A (Sender)", current_frame);
        imshow("Node B (Receiver)", reconstructed_frame);

        if (waitKey(1) == 27) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
