#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/SCTC_Codec.hpp"
#include "../include/UDP_Transport.hpp"

using namespace cv;
using namespace std;

int main() {
    cout << "[KALKI SENDER] Initializing Camera and UDP Tunnel on Port 8080..." << endl;
    
    VideoCapture cap(0); 
    if (!cap.isOpened()) { cerr << "FATAL: Cannot open camera." << endl; return -1; }

    int width = 640; int height = 480;
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);

    sctc::Encoder encoder(width, height, 15);
    sctc::UDPSender sender("127.0.0.1", 8080); // Sending to localhost for testing

    Mat current_frame;
    while (true) {
        cap.read(current_frame);
        if (current_frame.empty()) break;

        // Compress
        std::vector<uint8_t> payload = encoder.encode(current_frame.data);
        
        // Network Transport
        sender.send_payload(payload);

        putText(current_frame, "SENDER NODE (Capturing & Compressing)", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
        imshow("Drishti: Sender", current_frame);

        if (waitKey(1) == 27) break;
    }
    return 0;
}
