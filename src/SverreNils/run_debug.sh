#!/bin/bash
set -e

echo "🔨 Bygger operativsystemet..."
cmake --build build --target uiaos-create-image

echo "🚀 Starter QEMU med GDB-støtte..."
./scripts/start_qemu.sh build/kernel.iso build/disk.iso

echo ""
echo "🧠 Nå kan du trykke F5 i VS Code for å starte debugging (Qemu Debug)."
