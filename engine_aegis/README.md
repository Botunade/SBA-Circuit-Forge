# 🧠 SBA Aegis — AI Mentor Backend

**Primary Tech Stack:** Python 3.10+, FastAPI, Uvicorn, Google Protocol Buffers

## 📋 Overview
SBA Aegis operates as the isolated intelligence backend for the **SBA Circuit Forge** ecosystem. Functioning under a strict "teacher over the shoulder" philosophy, it reads incoming binary snapshots of the user's circuit to provide educational step-by-step diagnostic advice, warning messages, and target optimization tips.

## 🛑 Strict Operational Rules
1. **Zero Auto-Fixing:** Aegis is strictly forbidden from directly manipulating or automatically correcting the user's canvas layouts or netlist data. It must only provide descriptive chat telemetry and a list of structural component IDs to flash red. The user must apply all physical corrections manually.
2. **Zero JSON Policy:** All network communications to and from this backend must strictly pass serialized `application/x-protobuf` binary payloads. JSON formatting is prohibited for inter-engine data streams.
3. **Library Manifest Sync:** During initialization, Aegis caches a `LibraryManifest` detailing what SPICE subcircuits and assets exist locally on the client machine. The AI must never suggest components that fall outside this verified local catalog.

## 🛠️ Compilation & Local Setup
A boilerplate shell is already initialized in this folder. To open the communication pipeline:
1. Ensure your local machine has `protobuf-compiler` (`protoc`) installed.
2. Execute the pre-configured script to compile the shared data schema into native Python data classes:
   ```bash
   chmod +x compile_proto.sh
   ./compile_proto.sh
   ```
3. Install the minimal dependencies listed in requirements.txt:
   ```bash
   pip install -r requirements.txt
   ```
4. Fire up the development environment using Uvicorn:
   ```bash
   uvicorn main:app --reload
   ```

## 🎯 Immediate Milestones
* Complete the protobuf parser logic in main.py to correctly extract incoming client network streams.
* Construct the mock API responses for /api/watchdog_validate so the frontend can safely verify binary parsing before connecting live LLM token routing.