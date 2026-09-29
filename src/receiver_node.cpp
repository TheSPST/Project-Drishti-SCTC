#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/SCTC_Codec.hpp"
#include "../include/UDP_Transport.hpp"

using namespace cv;
using namespace std;

int main() {
    cout << "[KALKI RECEIVER] Listening for SCTC stream on UDP Port 8080..." << endl;
    
    int width = 640; int height = 480;
    sctc::Decoder decoder(width, height);
    sctc::UDPReceiver receiver(8080);

    Mat reconstructed_frame(height, width, CV_8UC3);

    while (true) {
        // Block until UDP frame is fully reassembled
        std::vector<uint8_t> payload = receiver.receive_payload();
        
        // Decompress
        decoder.decode(payload, reconstructed_frame.data);

        // Metrics
        long raw_kb = (width * height * 3) / 1024;
        long sctc_kb = payload.size() / 1024;
        if (sctc_kb == 0) sctc_kb = 1;

        putText(reconstructed_frame, "RECEIVER NODE (Decompressing via UDP)", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        
        char metrics[100];
        sprintf(metrics, "UDP Bandwidth: %ld KB | Ratio: %.1fx", sctc_kb, (float)raw_kb / sctc_kb);
        putText(reconstructed_frame, metrics, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 0), 2);

        imshow("Drishti: Receiver", reconstructed_frame);

        if (waitKey(1) == 27) break;
    }
    return 0;
}
