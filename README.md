### ⚠️ AI Engineering Notice: Live SPICE Integration
**For the AI Backend Developer (SBA Aegis):**
Please be advised that **SBA Pulse** has transitioned out of the mock testing phase. The C++ wrapper is now actively linked to the live `ngspice` repository.

* **What this means for you:** The `CircuitBlueprint` binary buffers your Python server generates are now being compiled into raw SPICE netlists and mathematically simulated on the local machine.
* **Data Shift:** The telemetry arrays (voltage/current nodes) that the C++ UI sends to your `/api/swoop_analysis` endpoint are no longer hardcoded dummy variables. They are live, fluctuating mathematical outputs. Ensure your LLM prompts and validation logic are prepared to handle dynamic, real-world electrical data for all future diagnostic responses.
