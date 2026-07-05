from fastapi import FastAPI, Request, Response
from fastapi.responses import Response

# Import the generated protobuf classes
from generated import circuit_pb2

app = FastAPI(title="SBA Aegis")

# In-memory manifest storage (Mock for now)
local_manifest = circuit_pb2.LibraryManifest()

@app.post("/api/manifest_sync")
async def manifest_sync(request: Request):
    """
    Accepts the LibraryManifest Protobuf from the C++ client on startup.
    Stores manifest in memory.
    """
    body = await request.body()
    manifest = circuit_pb2.LibraryManifest()
    manifest.ParseFromString(body)

    # Store locally
    global local_manifest
    local_manifest.CopyFrom(manifest)

    # Return a success message or just standard HTTP 200
    return Response(content=b"Manifest synced successfully", media_type="text/plain")

@app.post("/api/watchdog_validate")
async def watchdog_validate(request: Request):
    """
    Accepts the user's current canvas state (CircuitBlueprint).
    Returns a mock FaultDiagnostic buffer if an error is detected.
    """
    body = await request.body()
    blueprint = circuit_pb2.CircuitBlueprint()
    blueprint.ParseFromString(body)

    # Mock logic: return a hardcoded fault for demonstration
    fault = circuit_pb2.FaultDiagnostic()
    fault.status = "error"
    fault.fault_type = "short_circuit"
    fault.chat_message = "Warning: A potential short circuit is detected near the power supply module."

    # Mocking that component "COMP_1" is problematic
    if blueprint.components:
        fault.highlight_targets.append(blueprint.components[0].id)
    else:
        fault.highlight_targets.append("COMP_1")

    serialized_fault = fault.SerializeToString()
    return Response(content=serialized_fault, media_type="application/x-protobuf")

@app.post("/api/swoop_analysis")
async def swoop_analysis(request: Request):
    """
    Accepts a telemetry discrepancy and generates a chat response for the user.
    """
    # For now, just returning a static FaultDiagnostic with a chat response
    # Later this will be hooked up to google-generativeai
    fault = circuit_pb2.FaultDiagnostic()
    fault.status = "analysis_complete"
    fault.fault_type = "telemetry_discrepancy"
    fault.chat_message = "SBA Aegis Analysis: The target current was 3.0A, but actual is 2.5A. Please check the resistor values."
    fault.highlight_targets.append("RESISTOR_1")

    serialized_fault = fault.SerializeToString()
    return Response(content=serialized_fault, media_type="application/x-protobuf")
