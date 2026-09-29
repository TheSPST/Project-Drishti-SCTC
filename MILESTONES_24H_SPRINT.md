# 24-HOUR SPRINT: PROJECT DRISHTI LIVE PROTOTYPE
**Objective:** Evolve the SCTC mathematical proof-of-concept into a live, testable webcam video codec by tomorrow evening.

To build a testable prototype in 24 hours, we will temporarily bypass network transmission (UDP) and focus purely on the **Local Encoding/Decoding Pipeline** using your Mac's internal webcam. 

---

### ⏱️ MILESTONE 1: The Camera Hook (Morning)
*   **Goal:** Capture live video from the MacBook FaceTime HD Camera into a raw C++ matrix.
*   **The Tech:** We will use OpenCV for rapid prototyping, as writing raw AVFoundation code takes too long.
*   **Actionable Task:** 
    *   Run `brew install opencv`.
    *   Write `camera_capture.cpp` using `cv::VideoCapture(0)`.
    *   Convert the camera frame into a raw, flat RGB 1D array.

### ⏱️ MILESTONE 2: The SCTC Encoder Injection (Midday)
*   **Goal:** Replace the dummy values in `sctc_secure_core.cpp` with the real webcam RGB arrays.
*   **Actionable Task:**
    *   Capture Frame $T_0$ and hold it in RAM as the Baseline.
    *   Capture Frame $T_1$.
    *   Execute the `Threshold Quantization` noise-gate and Delta-subtraction logic on the live webcam pixels.
    *   Output the compressed SCTC byte array to a memory buffer.

### ⏱️ MILESTONE 3: The SCTC Decoder (Afternoon)
*   **Goal:** Reverse the math to reconstruct the video.
*   **Actionable Task:**
    *   Read the compressed SCTC byte buffer.
    *   Take the stored Baseline Frame and add the X, Y, and RGB deltas back into it.
    *   Output the fully reconstructed RGB frame.

### ⏱️ MILESTONE 4: The "Ghost Vision" Display Test (Evening)
*   **Goal:** Visually prove the compression works with zero noticeable quality loss.
*   **Actionable Task:**
    *   Use OpenCV `cv::imshow()` to display two side-by-side windows.
    *   **Window A:** The raw, uncompressed webcam feed.
    *   **Window B:** The SCTC Reconstructed feed.
    *   **The Terminal:** Continuously print the live bandwidth savings (e.g., *"Raw: 6 MB/s | SCTC: 0.8 MB/s"*).

---

### 🚀 HOW TO TEST IT TOMORROW
Once Milestone 4 is complete, you will run the app. 
1.  Sit perfectly still in front of the camera. The terminal should show a **90%+ compression ratio** because nothing is moving.
2.  Start talking and waving your hands. Watch the compression ratio adapt in real-time as the delta increases.
3.  Look at Window B to confirm the video quality has not degraded, proving the SCTC algorithm works flawlessly on physical light data.

*When you are ready to begin Milestone 1, we will write the OpenCV CMakeLists.txt and C++ capture script.*
