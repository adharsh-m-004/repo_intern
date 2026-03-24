# repo_intern
# Real-Time Audio Streaming Network (Intern Project)

A lightweight real-time bidirectional-capable audio communication system between two nodes using **PortAudio** for audio capture/playback and custom socket-based networking in C++.

---

## Project Architecture

The system follows a **Peer-to-Peer** audio streaming architecture with an optional web signaling layer:

- **Node A (Sender)**: Captures live audio from the microphone using PortAudio and sends raw PCM audio packets over the network.
- **Node B (Receiver)**: Receives the audio packets in real-time and plays them through the speakers using PortAudio.
- **Shared Layer**: `Shared.h` and `Network.hpp` contain common constants, packet structures, configuration, and networking utilities.
- **Web Layer**: 
  - `server.js` – Simple Node.js + Express server (for signaling, status monitoring).
  - `index.html` – Basic web frontend to interact with or monitor the streaming nodes.

**Data Flow**:
1. Node A opens input stream → reads audio buffers from microphone.
2. Audio data is packetized and sent over TCP/UDP sockets to Node B.
3. Node B receives packets → writes audio buffers to output stream (speakers).
4. Optional web dashboard shows connection status and logs.

**Key Design Decisions**:
- Raw PCM format (no compression) for lowest possible latency.
- Modular networking code in `Network.hpp`.
- Cross-platform audio handling via PortAudio.

---

## Technologies & 3rd-Party Libraries Used

| Component          | Library/Tool              | Purpose                                      |
|--------------------|---------------------------|----------------------------------------------|
| Audio Capture & Playback | **PortAudio**          | Cross-platform real-time audio I/O           |
| Networking         | Native C++ Sockets (Winsock2 on Windows) | TCP/UDP communication                |
| Dependency Management | **vcpkg**               | Easy installation of PortAudio               |
| Web Server         | Node.js + Express         | Signaling and simple web interface           |
| Frontend           | HTML + JavaScript         | Basic monitoring UI                          |

**Only major 3rd-party library**: PortAudio (included as `portaudio.dll` for Windows).

---

## Project Structure
repo_intern/
├── NodeA.cpp              # Sender node (Microphone → Network)
├── NodeB.cpp              # Receiver node (Network → Speakers)
├── Network.hpp            # Networking classes and helper functions
├── Shared.h               # Shared constants, packet structs, audio config
├── server.js              # Node.js backend server
├── index.html             # Web frontend
├── package.json           # Node.js dependencies
├── portaudio.dll          # PortAudio binary (Windows)
├── vcpkg/                 # vcpkg manifest and installed packages
├── NodeA.exe              # Pre-built sender executable
├── NodeB.exe              # Pre-built receiver executable
└── received_audio.raw     # Sample raw audio file (for testing)


---

## How to Execute the C++ Files

### Prerequisites
- Windows 10/11 (recommended)
- PortAudio (via vcpkg or pre-built dll)
- Node.js (optional, for web server)

### Option 1: Run Pre-built Executables (Recommended for Quick Test)

Open two separate terminals/command prompts:

```bash

npm install
node server.js
