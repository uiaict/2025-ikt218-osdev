#!/bin/bash
KERNEL_PATH=$1
DISK_PATH=$2

echo "Starting QEMU without GDB..."
qemu-system-i386 -boot d -hda $KERNEL_PATH -hdb $DISK_PATH -m 64 -serial stdio -k en-us

QEMU_PID=$!

# Function to check if gdb is running
is_gdb_running() {
    pgrep -f "gdb-multiarch" > /dev/null
}

# Function to handle termination signals
cleanup() {
    echo "Stopping QEMU..."
    kill $QEMU_PID
    exit 0
}

# Trap SIGINT and SIGTERM signals
trap cleanup SIGINT SIGTERM

# Wait for gdb to start
echo "Waiting for gdb to start..."
while ! is_gdb_running; do
    sleep 1
done
echo "gdb started."

# Wait for gdb to stop
echo "Monitoring gdb connection..."
while is_gdb_running; do
    sleep 1
done

# Cleanup after gdb stops
cleanup
