

KERNEL_PATH=$1
DISK_PATH=$2

if [[ ! -f "$KERNEL_PATH" ]]; then
    echo "❌ KERNEL_PATH not found: $KERNEL_PATH"
    exit 1
fi

if [[ ! -f "$DISK_PATH" ]]; then
    echo "❌ DISK_PATH not found: $DISK_PATH"
    exit 1
fi

echo "✅ Starting QEMU with PC-speaker support..."
qemu-system-i386 \
    -boot d \
    -hda "$KERNEL_PATH" \
    -hdb "$DISK_PATH" \
    -m 64 \
    -audiodev sdl,id=sdl1,out.buffer-length=40000 \
    -machine pcspk-audiodev=sdl1 \
    -serial stdio \
    -k en-us &

QEMU_PID=$!


cleanup() {
    echo "🛑 Stopping QEMU..."
    kill $QEMU_PID 2>/dev/null
    exit 0
}

trap cleanup SIGINT SIGTERM


wait $QEMU_PID
