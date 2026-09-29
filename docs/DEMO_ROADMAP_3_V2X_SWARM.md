# 6G DEMO ROADMAP: V2X Autonomous Swarm Nodes
**Feasibility Score: 7/10 (Best for Mobility/Automotive)**

## 1. The Concept
Vehicles sharing raw sensor data (LiDAR) in real-time to avoid collisions. Proves that SCTC allows high-fidelity sensor sharing even when vehicle-to-vehicle (V2V) radio links are constrained.

## 2. Hardware Requirements
*   **Vehicles:** 2x RC Car chassis (e.g., Donkey Car kits or Waveshare JetBot).
*   **Compute:** NanoPi R5C mounted on the chassis.
*   **Sensors:** RPLiDAR A1/A2 (2D LiDAR).
*   **Networking:** Wi-Fi 6 dongles for V2X side-link simulation.

## 3. Open-Source Software Stack
*   **Robotics Middleware:** ROS 2 (Humble/Iron).
*   **Navigation:** Nav2 for path planning and collision avoidance.
*   **Transport:** Eclipse Cyclone DDS or Zenoh.

## 4. SCTC Codec Integration
Wrap the SCTC codec to compress heavy 2D LiDAR point clouds and state-vectors (speed, trajectory). The NanoPi broadcasts this compressed state to the swarm. We can throttle the Wi-Fi to prove standard ROS2 crashes, while the SCTC-compressed swarm continues operating.
