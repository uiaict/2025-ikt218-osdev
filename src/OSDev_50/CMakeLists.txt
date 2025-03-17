########################################
# The University of Agder Operating System: UiAOS
# Languages: C, C++, and NASM Assembly
# Tip: Use Ctrl+Shift+P in Visual Studio Code to get started with CMake.
########################################

# Skip compiler self-tests (saves time, avoids errors with cross compilers)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# Set cross-compilation compilers before project declaration
set(CMAKE_C_COMPILER /usr/local/bin/i686-elf-gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/i686-elf-g++)
set(CMAKE_ASM_NASM_COMPILER nasm)

# Minimum required CMake version
cmake_minimum_required(VERSION 3.22.1)

# Project name and languages used
project(UiAOS LANGUAGES C CXX ASM_NASM)

# Create a lock file to prevent parallel runs of CMake
file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)

########################################
# CMake: Import Plugins
########################################
include(FetchContent)

########################################
# UiAOS: Variables
########################################
set(OS_ARCH_TARGET "i386")
set(OS_NAME "UiA Operating System")
set(OS_KERNEL_NAME "uiaos")
set(OS_KERNEL_BINARY "kernel.bin")
set(OS_KERNEL_IMAGE "kernel.iso")
set(OS_KERNEL_LINKER "${CMAKE_SOURCE_DIR}/src/arch/${OS_ARCH_TARGET}/linker.ld")

########################################
# Compiler Configuration
########################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

########################################
# Assembly Configuration
########################################
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS "s;S;asm")
if(OS_ARCH_TARGET STREQUAL "i386")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32")
elseif(OS_ARCH_TARGET STREQUAL "x86_64")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
endif()

# Configure NASM compiler command
set(CMAKE_ASM_NASM_COMPILE_OBJECT 
    "<CMAKE_ASM_NASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

########################################
# Kernel Target Configuration
########################################
# Add main kernel executable target
add_executable(uiaos-kernel 
    src/multiboot2.asm
    src/kernel.c
    src/gdt.c
    src/terminal/terminal.c
    src/arch/i386/gdt_flush.asm
)

# Configure include directories for kernel
target_include_directories(uiaos-kernel PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libc
)

# Set compiler flags for C and C++ files
target_compile_options(uiaos-kernel PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m32 -march=i386 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -Wno-main -g>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m32 -march=i386 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -g>
)

# Configure linker options for kernel
target_link_options(uiaos-kernel PUBLIC
    "-ffreestanding"
    "-nostdlib"
    "-static"
    "-pie"
    "-T${OS_KERNEL_LINKER}"
)

# Set kernel binary name
set_target_properties(uiaos-kernel PROPERTIES
    OUTPUT_NAME "${OS_KERNEL_BINARY}"
)

########################################
# Image Creation Configuration
########################################
# Set up directories for ISO creation
set(ISO_DIR ${CMAKE_CURRENT_BINARY_DIR}/iso)
set(LIMINE_CONFIG_DIR ${CMAKE_SOURCE_DIR})
set(LIMINE_DIR /usr/local/limine)
set(DISK_IMAGE "${CMAKE_CURRENT_BINARY_DIR}/disk.iso")

# Create FAT32 disk image target
add_custom_target(create-fat32-disk
    COMMAND dd if=/dev/zero of=${DISK_IMAGE} bs=1M count=32
    COMMAND mkfs.fat -F 32 ${DISK_IMAGE}
    VERBATIM
)

# Create bootable OS image target
add_custom_target(uiaos-create-image
    # Clean and create ISO directory
    COMMAND rm -rf ${ISO_DIR}
    COMMAND mkdir -p ${ISO_DIR}
    
    # Copy kernel and bootloader files
    COMMAND cp -v $<TARGET_FILE:uiaos-kernel> 
        ${LIMINE_CONFIG_DIR}/limine.cfg ${LIMINE_DIR}/limine-bios.sys ${LIMINE_DIR}/limine-bios-cd.bin
        ${LIMINE_DIR}/limine-uefi-cd.bin ${ISO_DIR}/
    
    # Set up EFI boot directory
    COMMAND mkdir -p ${ISO_DIR}/EFI/BOOT
    COMMAND cp -v ${LIMINE_DIR}/BOOTX64.EFI ${ISO_DIR}/EFI/BOOT/
    COMMAND cp -v ${LIMINE_DIR}/BOOTIA32.EFI ${ISO_DIR}/EFI/BOOT/
    
    # Create bootable ISO
    COMMAND xorriso -as mkisofs -b limine-bios-cd.bin
        -no-emul-boot -boot-load-size 4 -boot-info-table
        --efi-boot limine-uefi-cd.bin
        -efi-boot-part --efi-boot-image --protective-msdos-label
        ${ISO_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
    
    # Install BIOS bootloader
    COMMAND ${LIMINE_DIR}/limine bios-install ${CMAKE_CURRENT_BINARY_DIR}/kernel.iso
    DEPENDS create-fat32-disk uiaos-kernel
    VERBATIM
)