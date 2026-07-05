#!/bin/bash
# Compile protobufs for Python
mkdir -p engine_aegis/generated
python3 -m grpc_tools.protoc -I=shared_schemas --python_out=engine_aegis/generated shared_schemas/circuit.proto
echo "Protobuf compiled for Python successfully."