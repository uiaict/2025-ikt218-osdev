#!/bin/bash

KERNEL_PATH=$1
DISK_PATH=$2

# Kill any existing QEMU instances (avoids port conflicts or stale processes)
pkill -9 qemu-system-i386 2>/dev/null || true

# Start QEMU in the background with GDB stub enabled
echo "Starting QEMU"
qemu-system-i386 \
    -S -gdb tcp::1234 \
    -boot d \
    -hda "$KERNEL_PATH" \
    -hdb "$DISK_PATH" \
    -m 64 \
    -audiodev sdl,id=sdl1,out.buffer-length=40000 \
    -machine pcspk-audiodev=sdl1 \
    -serial pty \
    &

QEMU_PID=$!

# Function to check if GDB is running
is_gdb_running() {
    pgrep -f "gdb-multiarch" > /dev/null
}

# Function to handle termination
cleanup() {
    echo "Stopping QEMU..."
    kill $QEMU_PID 2>/dev/null
    exit 0
}

# Handle Ctrl+C and SIGTERM
trap cleanup SIGINT SIGTERM

# Wait for GDB to attach
echo "Waiting for GDB to start..."
while ! is_gdb_running; do
    sleep 1
done
echo "GDB started."

# Monitor GDB
echo "Monitoring GDB connection..."
while is_gdb_running; do
    sleep 1
done

# Cleanup once GDB exits
cleanup
