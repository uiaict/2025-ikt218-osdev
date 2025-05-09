#!/bin/bash
INPUT_ISO_PATH=$1 # Take ISO path as argument

# --- Determine path to disk.img relative to ISO path ---
# This assumes disk.img is in the same directory as kernel.iso (e.g., your build output dir)
ISO_DIR=$(dirname "$INPUT_ISO_PATH")
INPUT_DISK_PATH="$ISO_DIR/disk.img"
# Fallback if ISO_DIR is just '.'
if [ "$ISO_DIR" == "." ]; then
    INPUT_DISK_PATH="./disk.img"
fi


# Basic check for arguments
if [ -z "$INPUT_ISO_PATH" ]; then
  echo "Usage: $0 <path/to/kernel.iso>"
  exit 1
fi

# Determine the script's own directory absolutely
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

# Function to resolve path (handles both relative and absolute input)
resolve_path() {
  local input_path="$1"
  local base_dir="$2"
  # Check if input is potentially an absolute path
  if [[ "$input_path" == /* ]]; then
    # Use readlink -f directly on the absolute path
    readlink -f "$input_path"
  else
    # Input is relative, resolve it relative to base_dir
    readlink -f "$base_dir/$input_path"
  fi
}

# Calculate the absolute paths for ISO and DISK
ABS_ISO_PATH=$(resolve_path "$INPUT_ISO_PATH" "$SCRIPT_DIR")
ABS_DISK_PATH=$(resolve_path "$INPUT_DISK_PATH" "$SCRIPT_DIR")


# Check if readlink failed (returned empty string)
if [ -z "$ABS_ISO_PATH" ]; then
  echo "Error: Failed to resolve ISO path: $INPUT_ISO_PATH"
  exit 1
fi
if [ -z "$ABS_DISK_PATH" ]; then
   # If resolving relative to script dir failed, try relative to CWD as fallback
   ABS_DISK_PATH=$(resolve_path "$INPUT_DISK_PATH" "$PWD")
   if [ -z "$ABS_DISK_PATH" ]; then
      echo "Error: Failed to resolve disk path: $INPUT_DISK_PATH"
      exit 1
   fi
fi


echo "DEBUG: Script directory             : $SCRIPT_DIR"
echo "DEBUG: Input ISO Path Arg           : $INPUT_ISO_PATH"
echo "DEBUG: Calculated Absolute ISO Path  : $ABS_ISO_PATH"
echo "DEBUG: Inferred Input Disk Path    : $INPUT_DISK_PATH"
echo "DEBUG: Calculated Absolute Disk Path: $ABS_DISK_PATH"

# Check if files exist using the calculated ABSOLUTE paths
if [ ! -f "$ABS_ISO_PATH" ]; then
  echo "Error: ISO file not found at calculated absolute path: $ABS_ISO_PATH"
  exit 1
fi
echo "DEBUG: ISO file check passed using absolute path."
if [ ! -f "$ABS_DISK_PATH" ]; then
  echo "Error: Disk image file not found at calculated absolute path: $ABS_DISK_PATH"
  exit 1
fi
echo "DEBUG: Disk file check passed using absolute path."


# Start QEMU using the ISO with -cdrom AND the disk image with -hdb
echo "Starting QEMU with ISO (-cdrom=$ABS_ISO_PATH) and Disk (-hdb=$ABS_DISK_PATH), serial output to qemu_output.log"
qemu-system-i386 -S -gdb tcp::1234 \
                 -boot d \
                 -cdrom "$ABS_ISO_PATH" \
                 -hdb "$ABS_DISK_PATH" \
                 -m 1024 \
                 -audiodev sdl,id=sdl1,out.buffer-length=40000 -machine pcspk-audiodev=sdl1 \
                 -serial file:qemu_output.log &

QEMU_PID=$!

# --- Rest of the script (QEMU check, GDB wait, cleanup) ---

# Check if QEMU started successfully
sleep 1
if ! kill -0 $QEMU_PID 2>/dev/null; then
    echo "Error: QEMU failed to start."
    exit 1
fi
echo "QEMU started with PID $QEMU_PID"

# Function to check if gdb is running
is_gdb_running() {
    pgrep -f "gdb-multiarch.*1234" > /dev/null
}

# Function to handle termination signals
cleanup() {
    echo "Stopping QEMU (PID $QEMU_PID)..."
    kill $QEMU_PID 2>/dev/null
    sleep 1
    if kill -0 $QEMU_PID 2>/dev/null; then
        echo "QEMU did not terminate gracefully, sending SIGKILL..."
        kill -9 $QEMU_PID 2>/dev/null
    fi
    echo "QEMU stopped."
    # Log file is created relative to where QEMU was launched from
    if [ -f "./qemu_output.log" ]; then
        echo "--- QEMU Output Log (./qemu_output.log) ---"
        cat "./qemu_output.log"
        echo "-----------------------------------------"
    else
        echo "DEBUG: Log file ./qemu_output.log not found."
    fi
    exit 0
}

# Trap signals for cleanup
trap cleanup SIGINT SIGTERM EXIT

# Wait for gdb to connect
echo "Waiting for gdb connection (listening on tcp::1234)..."
while ! is_gdb_running; do
    if ! kill -0 $QEMU_PID 2>/dev/null; then
        echo "QEMU process exited before GDB connected."
        # Call cleanup explicitly to show logs if QEMU dies early
        cleanup
        exit 1
    fi
    sleep 1
done
echo "GDB process detected."

# Monitor the GDB connection / QEMU process
echo "Monitoring GDB/QEMU..."
while kill -0 $QEMU_PID 2>/dev/null; do
    if ! is_gdb_running; then
        echo "GDB process appears to have disconnected/exited."
        break
    fi
    sleep 2
done

echo "Monitoring loop finished."
# Cleanup will be called automatically due to the EXIT trap