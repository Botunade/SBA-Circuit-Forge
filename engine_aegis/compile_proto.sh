#!/bin/bash
# Script to compile protobuf schemas for the Python backend

SCHEMA_DIR="../shared_schemas"
OUT_DIR="."

echo "Compiling protobuf schema for Python in Aegis engine..."
protoc -I=$SCHEMA_DIR --python_out=$OUT_DIR $SCHEMA_DIR/circuit.proto

if [ $? -eq 0 ]; then
    echo "Successfully generated Python protobuf files."
else
    echo "Failed to generate Python protobuf files."
    exit 1
fi
