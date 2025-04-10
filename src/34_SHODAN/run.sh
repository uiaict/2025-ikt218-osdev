#!/bin/bash

# Navigate to script location (repo root safe)
cd "$(dirname "$0")"

echo "[+] Cleaning old build..."
rm -rf build

echo "[+] Configuring CMake with cross-compilers..."
cmake -S . -B build \
  -DCMAKE_C_COMPILER=/usr/local/bin/i686-elf-gcc \
  -DCMAKE_CXX_COMPILER=/usr/local/bin/i686-elf-g++ || {
    echo "CMake configure failed"; exit 1;
}

echo "[+] Building OS image..."
cmake --build build --target uiaos-create-image || {
    echo "Build failed"; exit 1;
}

echo "[+] Launching QEMU..."
# qemu-system-i386 -cdrom build/kernel.iso -boot d -m 64 -display gtk
qemu-system-i386 -cdrom build/kernel.iso -boot d -m 64 -serial stdio -display gtk


