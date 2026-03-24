#include "Shared.h"
#include "Network.hpp"
#include <portaudio.h>
#include <iostream>
#include <fstream>

int main() {
    // Initialize PortAudio
    if (Pa_Initialize() != paNoError) return 1;

    PaStream* stream;
    PaError err = Pa_OpenDefaultStream(&stream, 0, NUM_CHANNELS, paFloat32, 
                                        SAMPLE_RATE, FRAMES_PER_BUFFER, nullptr, nullptr);
    
    if (err != paNoError) return 1;
    Pa_StartStream(stream);

    // Initialize Network (Port 8080)
    UDPReceiver receiver(8080);
    AudioPacket packet;

    // Open file for raw recording
    std::ofstream outFile("received_audio.raw", std::ios::binary);

    // Main Loop
    while (true) {
        // 1. Receive Network Packet
        int bytesRead = receiver.Receive(&packet, sizeof(AudioPacket));
        
        if (bytesRead == sizeof(AudioPacket)) {
            // 2. Calculate Latency
            int64_t currentTime = GetCurrentTimeMs();
            int64_t transitDelay = currentTime - packet.timestampMs;
            
            // 3. API Output: Print ONLY JSON to stdout for the Node.js bridge
            // Note: Use std::flush to ensure the bridge receives it instantly
            std::cout << "{\"seq\":" << packet.sequenceNumber 
                      << ",\"delay\":" << transitDelay << "}" << std::endl;

            // 4. Save to disk
            if (outFile.is_open()) {
                outFile.write(packet.payload, PAYLOAD_SIZE);
            }

            // 5. Playback Real-time
            Pa_WriteStream(stream, packet.payload, FRAMES_PER_BUFFER);
        }
        
        // Optional: Add a break condition or signal handling here
    }

    // Cleanup
    outFile.close();
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
}