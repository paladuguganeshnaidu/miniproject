#!/usr/bin/env bash
# Exit on error
set -o errexit

# Install dependencies
pip install -r requirements.txt

# Compile C Code
echo "Compiling C Shared Library..."
gcc -shared -o backend/libbanckqueue.so backend/banckqueue.c -fPIC

echo "Build complete."
