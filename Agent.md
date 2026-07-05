# 🤖 SBA CIRCUIT FORGE: MASTER AGENT DIRECTIVE
**Project Lead:** Boluwatife | SBA Engineering
**Target Audience:** E&I Technicians & HND Engineering Students
**Agent Designation:** Google Jules / General Coding Agents

## ⚠️ SYSTEM OVERRIDE & CORE DIRECTIVES
As an autonomous coding agent operating in this repository, you must read and adhere to this document before generating any code, opening any Pull Request, or modifying any architecture.

This software relies on a strict "Teacher Over The Shoulder" philosophy.
**CRITICAL RULE:** The AI backend (SBA Aegis) is strictly forbidden from directly modifying the user's workspace, netlist, or automatically fixing component values. It acts *only* as a diagnostic mentor. The user must manually apply all fixes. Do not write any automation code that bypasses this rule.

---

## 🏛️ THE 3-PILLAR ARCHITECTURE
You are building three strictly isolated engines. Do not blur the lines between these environments.

1. **SBA Circuit Forge (Frontend Workspace)**
   * **Stack:** Native C++ / Qt Framework (`QGraphicsScene`).
   * **Rule:** Must remain ultra-lightweight (<4GB RAM footprint). Handles the 2D vector drawing and user interface.

2. **SBA Pulse (Local SPICE Engine)**
   * **Stack:** C++ wrapper for `ngspice.dll` (via `sharedspice.h`).
   * **Rule:** No GUI code. Compiles the Qt canvas data into raw SPICE netlists, executes the math locally, and streams live telemetry (voltage/current matrices).

3. **SBA Aegis (AI Mentor Backend)**
   * **Stack:** Python 3.10+ / FastAPI.
   * **Rule:** Isolated routing engine. Reads library manifests and circuit snapshots to provide chat-based diagnostics.

---

## 🔌 THE DATA CONTRACT (ZERO-JSON POLICY)
**JSON is strictly prohibited for inter-engine communication.**
All data moving between the C++ frontend (Pulse/Canvas) and the Python backend (Aegis) must be serialized using Google Protocol Buffers (`application/x-protobuf`).

* **Schema Location:** All schema definitions live in `/shared_schemas/circuit.proto`.
* **Agent Task:** If you modify a data structure, you must update the `.proto` file and recompile the bindings for BOTH C++ and Python. Do not update one without the other.

---

## 📂 REPOSITORY MAP
Maintain this exact folder structure. Do not create new root directories without explicit permission.

```text
/sba_circuit_forge
├── Agent.md             # This file.
├── README.md            # Public documentation & Changelog.
├── /shared_schemas      # .proto data contracts.
├── /engine_pulse        # C++ backend wrappers for ngspice.dll.
├── /engine_aegis        # Python FastAPI backend.
├── /ui_canvas           # C++ Qt graphical interface.
└── /assets
    ├── /spice_models    # Raw .SUBCKT files.
    └── /visuals         # SVG vector shapes.
```