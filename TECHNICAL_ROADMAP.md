# PROJECT DRISHTI: SCTC Zero-Jitter Video Codec
**Mission:** Eradicate H.264/HEVC latency by treating 2D video as sparse array mathematics over raw UDP.

---

## 1. TECHNICAL DEVELOPMENT PHASES

### Phase 1: Core Array Mathematics (Local Test)
*   **Objective:** Prove the O(1) pixel subtraction logic without network variables.
*   **Tasks:**
    1.  Use Apple AVFoundation / OpenCV to capture raw, uncompressed 1080p webcam frames.
    2.  Store Frame $T_0$ as the `Baseline_Matrix`.
    3.  For Frame $T_1$, execute $T_1 - T_0$.
    4.  Apply Ternary Quantization: Drop all $0$s. Pack the remaining moving pixels (X, Y, RGB_Delta) into a flat byte array.
    5.  Reconstruct and display the video locally.

### Phase 2: UDP Network Transport (The P2P Tunnel)
*   **Objective:** Transmit the SCTC delta arrays between two MacBooks over Wi-Fi.
*   **Tasks:**
    1.  Write a lightweight C++ UDP socket wrapper.
    2.  Chunk the delta-array into MTU-compliant micro-packets (under 1400 bytes) to avoid router fragmentation.
    3.  Implement a basic UDP packet ordering sequence on the receiver node.

### Phase 3: Hardware Acceleration (The Silicon Edge)
*   **Objective:** Drop encoding latency from milliseconds to microseconds.
*   **Tasks:**
    1.  Rewrite the C++ array subtraction using **ARM NEON SIMD** intrinsics (`vsubq_s16`).
    2.  Bypass standard OS rendering; pipe the reconstructed pixels directly into Apple's Metal API framework for zero-copy screen rendering.

---

## 2. CRITICAL BOTTLENECKS & ENGINEERING SOLUTIONS

### Bottleneck A: The "Camera Pan" Payload Spike
*   **The Threat:** SCTC assumes the background is static (90% zeros). If the user physically picks up the MacBook and moves it, the *entire* background changes simultaneously. The delta becomes 100% of the screen. The payload spikes to 300+ MB/s instantly, immediately crashing the UDP socket and the router.
*   **The Sovereign Solution (Adaptive I-Frame Reset):** 
    We implement a hard mathematical circuit breaker. If the C++ engine detects that the delta payload exceeds 15% of the total frame, it instantly halts delta-encoding. It transmits a single "Baseline Reset Flag," sends a fresh, heavily quantized full frame (I-Frame), and immediately resumes delta-tracking on the new background.

### Bottleneck B: CMOS Sensor Noise (The Fake Delta)
*   **The Threat:** Physical webcam sensors have electrical grain. Even if you are sitting perfectly still against a blank wall, the RGB value of the wall pixels will fluctuate randomly by $\pm 1$ or $\pm 2$ every single frame due to light noise. A naive delta-subtraction will register the entire wall as "moving," ruining the compression ratio.
*   **The Sovereign Solution (Threshold Quantization):**
    We introduce a Noise Gate before compression. 
    `if (abs(Pixel_Current - Pixel_Baseline) < NOISE_THRESHOLD) { Delta = 0; }`
    By forcing minor electrical noise to equal absolute zero, we force the background to remain mathematically static, restoring the 10x compression ratio without any visible loss in video quality to the human eye.

### Bottleneck C: UDP Packet Loss (The Ghost Pixel)
*   **The Threat:** UDP does not guarantee delivery. If a packet containing the delta for your right eye is dropped by the router, your eye will freeze on the receiver's screen while the rest of your face moves, causing a "Ghost Pixel" tearing effect.
*   **The Sovereign Solution (Rolling Baseline Refresh):**
    Instead of relying purely on frame-by-frame deltas forever, the sender slowly sweeps a hidden full-frame refresh from top to bottom. It sends 1% of the absolute baseline pixels mixed into the delta stream every frame. Over 100 frames, the entire screen corrects itself silently, instantly healing any dropped UDP packets without causing a network spike.
