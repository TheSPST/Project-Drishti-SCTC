# 6G DEMO ROADMAP: ISAC (Integrated Sensing and Communication)
**Feasibility Score: 7/10 (Best for Deep-Tech/Telecom)**

## 1. The Concept
Use standard radio frequencies (Wi-Fi) to detect human movement or gestures (Cell Tower Radar). Demonstrates how SCTC handles massive, raw I/Q or CSI telemetry streams at the edge.

## 2. Hardware Requirements
*   **Edge Node:** NanoPi R5C.
*   **Radios:** Software Defined Radio (PlutoSDR / HackRF One) OR compatible Broadcom Wi-Fi dongles for CSI extraction.

## 3. Open-Source Software Stack
*   **SDR Processing:** GNU Radio (`gr-iio` or `gr-osmosdr`).
*   **Wi-Fi Sensing:** Nexmon CSI Extractor (if using Wi-Fi instead of SDR).

## 4. SCTC Codec Integration
SDRs generate massive amounts of uncompressed I/Q data. The NanoPi uses the SCTC lossless codec to compress this raw data stream locally and pipes it over the network to a heavy compute node (e.g., a Mac M1) to run the actual radar-detection machine learning models.
