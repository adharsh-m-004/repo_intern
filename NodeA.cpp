#include "Shared.h"
#include "Network.hpp"
#include <portaudio.h>
#include <iostream>

int main() {
    if (Pa_Initialize() != paNoError) return 1;

    PaStream* stream;
    Pa_OpenDefaultStream(&stream, NUM_CHANNELS, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, nullptr, nullptr);
    Pa_StartStream(stream);

    UDPSender sender("10.215.97.37", 8080); 
    AudioPacket packet;
    packet.sequenceNumber = 0;

    while (true) {
        // 1. Capture Audio
        Pa_ReadStream(stream, packet.payload, FRAMES_PER_BUFFER);

        // 2. Metadata
        packet.timestampMs = GetCurrentTimeMs();
        
        // 3. Transmit
        sender.Send(&packet, sizeof(AudioPacket));

        // 4. API Output: Pulse status every 100 packets to show it's alive
        if (packet.sequenceNumber % 100 == 0) {
            std::cout << "{\"status\":\"sending\", \"seq\":" << packet.sequenceNumber << "}" << std::endl;
        }

        packet.sequenceNumber++;
    }

    Pa_Terminate();
    return 0;
}