# 6G DEMO ROADMAP: Non-Terrestrial LEO CubeSat
**Feasibility Score: 7/10 (Best for Aerospace/Defense)**

## 1. The Concept
Emulate a high-efficiency narrowband space-to-ground link. Proves that SCTC can force massive throughput over extreme narrowband RF connections used in space.

## 2. Hardware Requirements
*   **CubeSat Node:** NanoPi R5C.
*   **Ground Station:** RTL-SDR dongle or Desktop.
*   **RF Link:** USB LoRa transceivers (e.g., SX1262 / SX1276) to emulate the low-bandwidth, long-range nature of satellite communications.

## 3. Open-Source Software Stack
*   **Flight Software:** NASA F´ (F Prime) or KubOS.
*   **Signal Processing:** GNU Radio.

## 4. SCTC Codec Integration
A completely static, tabletop demo. The NanoPi acts as the satellite, compressing high-res imaging data or telemetry using SCTC. It pushes this tiny payload over the slow LoRa connection. The Ground Station decompresses it, proving what would normally take 10 minutes to download from orbit now takes 10 seconds.
