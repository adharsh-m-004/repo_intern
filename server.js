const express = require('express');
const { spawn, exec } = require('child_process');
const http = require('http');
const { Server } = require('socket.io');

const app = express();
const server = http.createServer(app);
// Use origin: "*" so Live Server and local files can both connect
const io = new Server(server, { cors: { origin: "*" } });

// Keep track of the active processes globally
let runningProcesses = {
    sender: null,
    receiver: null
};

io.on('connection', (socket) => {
    console.log('User connected to control panel');

    socket.on('start-node', (mode) => {
        // 1. If this specific mode is already running, don't start it again
        if (runningProcesses[mode]) {
            console.log(`${mode} is already running.`);
            return;
        }

        const exe = mode === 'sender' ? './NodeA.exe' : './NodeB.exe';
        console.log(`Spawning ${mode}...`);
        
        runningProcesses[mode] = spawn(exe);

        // 2. Pipe the C++ JSON output to ALL connected tabs
        runningProcesses[mode].stdout.on('data', (data) => {
            const lines = data.toString().split('\n');
            lines.forEach(line => {
                if (line.trim().startsWith('{')) {
                    try {
                        const json = JSON.parse(line);
                        // Broadcast to everyone
                        io.emit('node-data', { ...json, type: mode });
                    } catch (e) { }
                }
            });
        });

        // 3. Handle process exit/cleanup
        runningProcesses[mode].on('close', (code) => {
            console.log(`${mode} exited with code ${code}`);
            runningProcesses[mode] = null;
            io.emit('node-status', { mode, running: false });
        });

        // Notify all tabs that this mode is now running
        io.emit('node-status', { mode, running: true });
    });

    socket.on('stop-node', () => {
        console.log('Stopping all nodes...');
        if (runningProcesses.sender) runningProcesses.sender.kill();
        if (runningProcesses.receiver) runningProcesses.receiver.kill();
    });
});

server.listen(5000, () => console.log('Bridge running on port 5000'));