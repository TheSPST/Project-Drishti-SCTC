# 6G DEMO ROADMAP: Holographic Telepresence
**Feasibility Score: 7/10 (Highest Wow-Factor)**

## 1. The Concept
Compress and transmit live 3D volumetric data (point clouds) in real-time. This visually proves SCTC's massive payload reduction capabilities for 6G bandwidth limits.

## 2. Hardware Requirements
*   **Edge Compute:** 2x NanoPi R5C (Sender and Receiver).
*   **Sensor:** Intel RealSense depth camera (D435i or D455) via USB 3.0.
*   **Display:** 3D Holographic LED Fan or standard monitor.

## 3. Open-Source Software Stack
*   **Capture:** `librealsense` SDK and Point Cloud Library (PCL).
*   **Framework:** ROS 2 (Humble) with `realsense2_camera` wrapper.
*   **Transport:** ROS 2 over Eclipse Zenoh (optimized for low-overhead networking).

## 4. SCTC Codec Integration
Create a custom ROS 2 node running the C++ SCTC codec. The node ingests raw RGB-D arrays from the RealSense camera, compresses them via ARM NEON SIMD optimizations, transmits over UDP/Zenoh, and decompresses on the receiving NanoPi before rendering.
