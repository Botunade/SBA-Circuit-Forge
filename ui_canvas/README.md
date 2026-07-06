# 🖥️ SBA Circuit Forge — Desktop UI Canvas

**Primary Tech Stack:** Native C++, Qt Framework (`QGraphicsScene`, `QGraphicsView`, `QNetworkAccessManager`)

## 📋 Overview
SBA Circuit Forge acts as the primary master application executable and the visual gateway for the entire system. It provides an optimized, infinite 2D vector workspace where technicians and engineering students draft circuits, interact with measurement instruments, and consult the AI feedback terminal.

## ⚙️ Core Responsibilities
1. **The Vector Canvas (`QGraphicsScene`):** Manages interactive elements (`SbaCircuitModule` objects), mouse events, node wiring lines, and snapping matrices.
2. **Data Streaming Client:** Houses the unified `QNetworkAccessManager` engine to ship compiled canvas data matrices directly to the SBA Aegis backend via binary Protobuf payloads.
3. **Live Fault Visualization:** Implements event targets to receive `FaultDiagnostic` packages. When the AI signals an error status, the canvas captures the target IDs and instantly triggers standard layout alterations (e.g., painting problematic wire paths or components bright red).
4. **Instrument Rendering:** Intercepts raw telemetry data vectors pumped asynchronously from SBA Pulse, rendering live wave parameters onto the virtual oscilloscope and multimeter panels smoothly.

## 🛑 Hardware & Performance Targets
* **Memory Optimization:** The master application loop must operate with ultra-lean memory footprints. The entire desktop layout must idle comfortably well below a **4GB RAM limit**.
* **Thread Boundaries:** Do not run heavy network calls or SPICE simulation step-throughs directly inside the primary GUI thread. Keep drawing operations fluid and responsive by dispatching heavy input/output parsing to worker routines.