#!/bin/bash

# Leee Project Test Runner

echo "=== Leee Unit Tests Runner ==="
echo ""

# Check if test directory exists
if [ ! -d "test" ]; then
    echo "Error: test directory not found!"
    exit 1
fi

cd test

# Run build and test script
./build_and_run.sh

if [ $? -eq 0 ]; then
    echo ""
    echo "=== All Tests Completed Successfully ==="
else
    echo ""
    echo "=== Tests Failed ==="
    exit 1
fi
