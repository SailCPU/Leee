#!/bin/bash

# Leee Unit Tests Build and Run Script

echo "=== Building ByteArray Unit Tests ==="

# Clean previous build
rm -f test_bytearray

# Compile test
g++ -std=c++17 -I../include -I../header-only -o test_bytearray test_bytearray.cpp

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo ""
echo "=== Running Tests ==="

# Run tests
./test_bytearray

if [ $? -eq 0 ]; then
    echo ""
    echo "All tests passed! ✓"
else
    echo ""
    echo "Some tests failed! ✗"
    exit 1
fi
