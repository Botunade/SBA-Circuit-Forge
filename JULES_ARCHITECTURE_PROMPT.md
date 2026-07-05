# SYSTEM PROMPT & ARCHITECTURE BLUEPRINT: SBA CIRCUIT FORGE
**Target Agent:** Google Jules (via Gemini 3 Pro)
**Project Lead:** Boluwatife (SBA Engineering)

## 1. Project Manifesto & Core Directives
Jules, your task is to scaffold, configure, and implement the foundational modular architecture for **SBA Circuit Forge**, an industrial-grade desktop engineering application designed for HND engineering students and E&I technicians.

The software strictly follows a "teacher over the shoulder" philosophy. It consists of a C++ frontend UI, a local SPICE simulation wrapper (**SBA Pulse**), and a Python-based AI backend (**SBA Aegis**).

**Strict Constraints:**
* **Zero AI Auto-Fixing:** SBA Aegis is forbidden from automatically altering the user's C++ canvas or netlist. It must only provide chat-based diagnostics, mathematical target values, and visual highlight coordinates. The user must manually apply all fixes.
* **API Isolation:** The system must be built with strict modularity. SBA Pulse (C++ DLL wrapper) and SBA Aegis (Python FastAPI) must operate as independent engines communicating purely via Protocol Buffers.
* **Hardware Ceiling:** The entire desktop footprint must idle under 4GB of RAM. Do not use Electron or heavy web-wrappers. The frontend must be native C++ utilizing the Qt framework.
* **Future-Proofing:** Structure the C++ component classes to easily accept ESP32 microcontrollers (programmed via C++ in PlatformIO) and custom sensor modules in Version 2.

---

## 2. Architectural Folder Structure
Initialize the repository with the following strict boundaries. You will generate the foundational boilerplate for each of these directories.

* `/sba_circuit_forge` (Root)
  * `/shared_schemas` (Contains the `.proto` files)
  * `/engine_pulse` (C++ SBA Pulse - Ngspice wrapper and network client)
  * `/engine_aegis` (Python FastAPI - SBA Aegis AI routing and logic)
  * `/ui_canvas` (C++ Qt frontend - QGraphicsScene and chat interface)
  * `/assets`
    * `/spice_models` (Raw `.SUBCKT` text files for modules)
    * `/visuals` (SVG files for generic components)

---

## 3. The Universal Data Contract (Protocol Buffers)
Do not use JSON for inter-engine communication. All data between the Qt frontend and the Python backend must be serialized using Google Protocol Buffers.

Initialize `shared_schemas/circuit.proto` with the following schema:

```protobuf
syntax = "proto3";
package sba_architecture;

message Pin {
  string name = 1;
  float relative_x = 2;
  float relative_y = 3;
}

message ComponentDef {
  string id = 1;
  string type = 2;
  float canvas_x = 3;
  float canvas_y = 4;
  repeated Pin pins = 5;
}

message CircuitBlueprint {
  string project_name = 1;
  repeated ComponentDef components = 2;
}

message FaultDiagnostic {
  string status = 1;
  string fault_type = 2;
  string chat_message = 3;
  repeated string highlight_targets = 4;
}

message LibraryManifest {
  repeated string installed_spice_models = 1;
  repeated string installed_svg_assets = 2;
}
```
Implement the CMake instructions to automatically compile this .proto file into C++ headers within /engine_pulse, and provide the protoc build script to generate the Python _pb2.py files in /engine_aegis.

## 4. Engine 1: SBA Pulse (The Local Simulation Wrapper)
SBA Pulse is the C++ engine that handles the ngspice.dll integration. It must be decoupled from the Qt UI, acting as a standalone API that the UI calls.
**Implement SBA_Pulse_Core.h and SBA_Pulse_Core.cpp with the following capabilities:**
 1. **DLL Linkage:** Implement the standard Ngspice shared library callback structs (SendChar, SendStat, ControlledExit, SendData, SendInitData).
 2. **Netlist Compilation:** Write a function generate_netlist(std::vector<ComponentDef> components) that parses the Protobuf data into a valid SPICE text string.
 3. **Real-Time Telemetry:** In the SendData callback, parse the live voltage and current vectors. Create a fast C++ event dispatcher that the Qt UI can subscribe to for updating the virtual multimeter and oscilloscope.
 4. **Live Parameter Tuning:** Implement a function alter_parameter(std::string target, double value) that uses the Ngspice alter command to change component values on the fly (e.g., simulating a user turning a potentiometer to adjust a precision heating system target to 120°C).

## 5. Engine 2: SBA Aegis (The AI Backend)
SBA Aegis is a Python FastAPI server. It must consume and return raw application/x-protobuf binary payloads.
**Implement the FastAPI application in /engine_aegis/main.py:**
 1. **Environment Setup:** Create a requirements.txt including fastapi, uvicorn, protobuf, and google-generativeai.
 2. **Endpoint 1: /api/manifest_sync:** Accepts the LibraryManifest Protobuf from the C++ client on startup. SBA Aegis must store this manifest in memory to ensure it never suggests a component the user does not have locally.
 3. **Endpoint 2: /api/watchdog_validate:** Accepts the user's current canvas state. Implement mock logic that immediately returns a serialized FaultDiagnostic buffer if an error is detected, populating the highlight_targets array with the offending component IDs.
 4. **Endpoint 3: /api/swoop_analysis:** Accepts a telemetry discrepancy (e.g., target 3.0A, actual 2.5A). Implement the LLM structured output routing that generates the step-by-step chat response for the user to manually apply the fix.

## 6. The UI Layer: Qt Frontend Canvas
The workspace must be built using C++ and the Qt framework.
**Implement the following Qt architecture in /ui_canvas:**
 1. **The Canvas:** Scaffold a QGraphicsScene and a QGraphicsView to serve as the infinite 2D engineering grid.
 2. **The Module Class:** Implement a custom QGraphicsItem named SbaCircuitModule. It must read the ComponentDef Protobuf data to draw its bounding rectangle and place its interactive pins.
 3. **The Watchdog Highlight:** Implement a C++ slot on_fault_received(std::vector<std::string> targets). When triggered by SBA Aegis, this function must iterate through the QGraphicsScene, locate the items matching the IDs, and temporarily change their QPen color to red.
 4. **The Network Manager:** Implement a QNetworkAccessManager singleton that handles POST requests to the SBA Aegis Python server, ensuring headers are set correctly for binary Protobuf transmission.

## 7. Execution Priorities for Jules
To begin this build, execute the tasks in the following order:
 1. Initialize the directory structure and write the CMakeLists.txt for the C++ modules.
 2. Generate and compile the circuit.proto file for both Python and C++.
 3. Scaffold the Python FastAPI endpoints in /engine_aegis and ensure they successfully return hardcoded binary buffers (Mock Mode).
 4. Scaffold the Qt QGraphicsScene and network manager to prove the C++ application can successfully request and deserialize the Protobuf data from the Python server.
Commit these foundational blocks to the repository so the engineering team can begin mapping the specific SPICE assets.