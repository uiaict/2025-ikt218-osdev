# Operating System Development Progress Documentation

## Introduction

In our Operating System (OS) class, we embarked on a project to develop a basic operating system, named **UiA Operating System (UiAOS)**. This document outlines the progress made, starting from foundational concepts up to the current development stage.

## Project Structure and Initial Setup

Our project is structured systematically to support clarity and ease of debugging and further enhancements. The initial setup utilizes various configurations and files as follows:

### Programming Languages and Technologies

- **C and Assembly (NASM):** Our OS uses C for core functionality and NASM Assembly for low-level bootloader and hardware interface tasks.
- **CMake** is utilized for project configuration and build automation, ensuring compatibility and reproducibility of the build environment across various systems.

### Project Configuration

- **CMakeLists.txt:**
  - Specifies that our OS targets the `i386` architecture.
  - Uses NASM Assembly (`elf32`) for assembly files.
  - Sets critical compiler flags (`-m32`, `-march=i386`, etc.) to ensure the kernel operates correctly in a low-level, isolated environment.
  - Defines our kernel binary as `kernel.bin`.

## Libraries and Standard Definitions

For simplicity and system compatibility, standard library headers (`libc`) were created:

- **stdint.h:** Defines standard integer types for portability.
- **stddef.h:** Contains standard definitions including size types and NULL pointer.
- **stdbool.h:** Defines boolean values (`true`, `false`).
- **stdarg.h:** Implements handling of variadic functions using GCC built-in macros.
- **limits.h, stdio.h, string.h:** Provide fundamental functions and type limits.

These files create a minimal standard library interface allowing basic functionalities typically expected in higher-level programs.

## Kernel Structure

The kernel's main responsibility is system management and hardware abstraction. At this initial stage, our kernel (`kernel.c`) has the following simplistic structure:

```c
#include "libc/stdint.h"
#include "libc/stddef.h"

int main(uint32_t magic, void* mb2_structure) {
    // Initialization code will be here.
    return 0;
}
```

This is the minimal kernel structure designed to handle startup and further loading of functionalities and services.

## Multiboot2 Bootloader

We adopted **Multiboot2** standard, implemented through assembly (`multiboot2.asm`) for booting our OS. Multiboot provides a standardized environment for the kernel:

- Defines system boot procedures.
- Ensures compatibility with bootloaders like GRUB2.
- Provides essential boot-time information (memory map, hardware details).

### Multiboot Information:

The multiboot structure (`multiboot2.asm`) specifies necessary headers and data structure conforming to Multiboot2 specifications.

## Kernel Linker Script

The linking process is controlled by `linker.ld`, ensuring correct memory layout and organization of kernel executable files, critical for the booting stage:

- Positions segments correctly in memory (`.text`, `.data`, `.bss`).
- Ensures correct loading and execution by the bootloader.

## System Call and I/O (Initial Stage)

At this initial stage, basic I/O operations are managed through:

- **stdio.h:** Defines basic output functions (`print`, `printf`).

Currently implemented functions include:

```c
int printf(const char* format, ...);
bool print(const char* data, size_t length);
int putchar(int character);
```

These functions serve as essential utilities to output text to the screen, assisting in debugging and user interaction during boot and kernel operation.

## Development Environment (Docker & Remote Setup)

To standardize the development across different team members, we have adopted a Docker-based remote development environment (`devcontainer.json`):

- Based on **Ubuntu Linux**, ensuring consistency across different developer machines.
- Contains pre-installed utilities and dependencies.
- Integrates seamlessly with Visual Studio Code, including essential plugins for debugging and code writing (CMake Tools, Copilot, GDB Debugger, etc.).

### Key Docker Features:

- **Cross-platform compatibility:** Linux, macOS, and Windows (via WSL2).
- **Graphical (X11) and audio support**: Ensures a seamless development experience regardless of host OS.
- **Automation:** `postStartCommand` executes startup scripts ensuring immediate readiness.

---

This section provided a comprehensive overview of our project's foundational setup and current status. Next, we will dive deeper into VGA text mode handling, the implementation of interrupts, and VGA driver interaction for more detailed visual outputs.

