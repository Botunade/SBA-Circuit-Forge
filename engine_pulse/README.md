# ⚡ SBA Pulse — Local Simulation Engine

**Primary Tech Stack:** Native C++, Ngspice Shared Library Interface (`sharedspice.h`)

## 📋 Overview
SBA Pulse serves as the raw mathematical core of the platform. Operating strictly as a headless C++ worker module, it has no graphical footprint. Its sole responsibility is to ingest structural canvas states, compile them into standard SPICE netlist syntax, execute transient or operating point math locally, and pump the live telemetry back up to the frontend UI loop.

## ⚙️ Core Responsibilities
1. **Netlist Compilation:** Transforms the structural `CircuitBlueprint` objects received from the frontend into raw, linearly ordered SPICE netlists.
2. **Asynchronous Telemetry Output:** Hooks directly into the native Ngspice API callback configurations (`SendData`, `SendInitData`) to intercept voltage and current vector streams on every simulation tick without blocking or stalling execution.
3. **Dynamic Parameter Modification:** Implements the runtime `alter` command bindings, allowing the simulation parameters to react immediately when users change manual values (e.g., dialing a potentiometer or constraining an operational target to 120°C).

## 🛠️ Build System Integration
The root configuration manages the build tree for this project.
* **Dependency Hook:** The current `CMakeLists.txt` is configured to leverage `FetchContent` to track the upstream `ngspice` source code mirror natively.
* **Hollow Mock Shell:** While in the primary scaffolding layout, the engine's SPICE interactions run via stubbed mock routines. This lets you test compilation safety, memory allocations, and network connectivity safely before mapping physical binaries onto a target machine.

## 🚀 Scalability Design Guardrail
All logic must remain strictly decoupled from the presentation layouts. Version 2 of SBA Circuit Forge introduces integrated hardware emulation for ESP32 microcontrollers. This engine must be structurally prepared to easily handle automated mixed-signal timing maps (co-simulating digital firmware logic from a PlatformIO environment alongside analog SPICE tracks).