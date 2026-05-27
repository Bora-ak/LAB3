# LAB3 UDP + Protobuf Socket App

## Overview

This repository contains a small C++ networking project that demonstrates how to exchange Protocol Buffers messages over UDP. It builds a shared networking library and two executable examples: a UDP server and a UDP client.

The current CMake build uses the sources in `apps/`, `src/`, `include/`, and `proto/simple_message.proto`. The repository also includes additional root-level sample files (`client.cpp`, `server.cpp`, and `proto/carstatus.proto`) that appear to be earlier or separate experiments and are not part of the active CMake targets.

## What The Project Does

The project sends a serialized protobuf message from a client to a server over UDP on port `9000`.

- The client creates a `lab3::SimpleMessage` protobuf message.
- The message is serialized into bytes and sent to `127.0.0.1:9000`.
- The server receives the UDP packet, parses the protobuf payload, and prints the sender name.
- The server sends back another protobuf message as a reply.
- The client receives the response and prints the returned sender name.

## Key Features

- UDP-based client/server communication in C++
- Protocol Buffers serialization and deserialization
- Reusable `UdpSocket` wrapper for bind, send, and receive operations
- CMake-based build setup
- Nix packaging support through `flake.nix` and `default.nix`

## Tech Stack

- Language: C++
- Build system: CMake
- Serialization: Protocol Buffers
- Packaging/environment: Nix
- Networking: POSIX UDP sockets

## Project Structure

```text
.
|-- apps/
|   |-- client.cpp          # Active client entry point
|   `-- server.cpp          # Active server entry point
|-- include/
|   `-- udp.hpp             # UDP socket interface
|-- src/
|   `-- udp.cpp             # UDP socket implementation
|-- proto/
|   |-- simple_message.proto
|   `-- carstatus.proto
|-- generated/
|   |-- carstatus.pb.cc
|   `-- carstatus.pb.h
|-- CMakeLists.txt
|-- default.nix
`-- flake.nix
```

## Main Build Targets

- `start_server`: launches the UDP server
- `start_client`: launches the UDP client
- `lab3net`: static library containing the UDP helper and generated protobuf sources

## Requirements

To build the project with CMake, you will need:

- A C++ compiler
- C++17 support
- CMake 3.16 or newer
- Protocol Buffers development libraries and compiler
- Matching `protoc` and `libprotobuf` versions

If you use Nix, the repository also provides `flake.nix` and `default.nix`.

## Installation And Build

### Option 1: Build With CMake

```bash
cmake -S . -B build
cmake --build build
```

If your environment has multiple protobuf installations, make sure the `protoc` binary used during code generation matches the installed protobuf headers and libraries.

### Option 2: Build With Nix

```bash
nix build
```

The Nix build produces a `result/` symlink that points to the built package output.

## How To Run

Open two terminals from the project root.

Start the server first:

```bash
./build/start_server
```

Then run the client:

```bash
./build/start_client
```

### Expected Example Flow

Server output should look similar to:

```text
Server listening on UDP :9000
Received from 127.0.0.1:<port> -> sender_name="client-bora"
```

Client output should look similar to:

```text
Server replied: "server-ht08"
```

## Configuration Notes

- The server binds to UDP port `9000`.
- The client sends to `127.0.0.1:9000`.
- There is no `.env`-based configuration in the current codebase.
- Sender names are currently hardcoded in the example sources:
  - Client: `client-bora`
  - Server reply: `server-ht08`

## Output / Results

This project does not generate reports or files during normal execution. Its primary output is console logging that shows:

- whether the client successfully sent a protobuf message
- whether the server successfully parsed the incoming payload
- whether the client received and parsed the reply

## Notes And Assumptions

- The README describes the targets defined in `CMakeLists.txt`, since those are the main executable entry points for the current build.
- The root-level `client.cpp` and `server.cpp` are not referenced by `CMakeLists.txt`, so they are treated here as non-primary sample files.
- The checked-in `generated/` directory contains protobuf output related to `carstatus.proto`, not to the currently built `simple_message.proto` target.

## Troubleshooting

- If the build fails with protobuf compatibility errors, verify that `protoc`, protobuf headers, and the linked protobuf library come from the same version.
- If the compiler reports missing C++17 types or features, build with a toolchain that supports C++17 and ensure your CMake configuration uses that standard.

## Future Improvements

- Add command-line options for host, port, and sender name
- Support repeated message exchange instead of a single request/response cycle
- Add automated tests for serialization and UDP communication
- Clarify or remove unused sample files to reduce confusion
- Generate all protobuf artifacts through one consistent build path

## License

No license file is currently included in the repository. If you plan to publish or share this project, consider adding a license such as MIT, Apache-2.0, or GPL based on your intended use.
