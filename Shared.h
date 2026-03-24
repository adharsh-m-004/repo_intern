#pragma once
#include <cstdint>
#include <chrono>

// Audio Settings
constexpr int SAMPLE_RATE = 44100;
constexpr int FRAMES_PER_BUFFER = 512;
constexpr int NUM_CHANNELS = 1;

// Packet Design
constexpr size_t PAYLOAD_SIZE = FRAMES_PER_BUFFER * sizeof(float);

struct AudioPacket {
    uint32_t sequenceNumber;
    int64_t timestampMs; // Used for delay calculation
    char payload[PAYLOAD_SIZE];
};

// Utility for getting current time in milliseconds
inline int64_t GetCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}