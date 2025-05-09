cmake_minimum_required(VERSION 3.16)
project(kernel C ASM)

# Set compiler explicitly
set(CMAKE_C_COMPILER i386-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)

# Important Directories
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(OBJ_DIR ${BUILD_DIR}/obj)
set(ISO_DIR ${BUILD_DIR}/isodir)
file(MAKE_DIRECTORY ${OBJ_DIR})

# Include directories
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/kernel/include
    ${CMAKE_SOURCE_DIR}/drivers/include
    ${CMAKE_SOURCE_DIR}/memory/include
)

# Automatically add any app include directories
file(GLOB APP_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/apps/*/include)
list(APPEND INCLUDE_DIRS ${APP_INCLUDE_DIRS})

# Compiler flags
set(CUSTOM_C_FLAGS -std=gnu99 -ffreestanding -O0 -Wall -Wextra)
set(CUSTOM_INCLUDE_FLAGS "")
foreach(INC_DIR ${INCLUDE_DIRS})
    list(APPEND CUSTOM_INCLUDE_FLAGS -I${INC_DIR})
endforeach()

# Collect all source files
file(GLOB_RECURSE C_SOURCES
    ${CMAKE_SOURCE_DIR}/kernel/src/*.c
    ${CMAKE_SOURCE_DIR}/drivers/src/*.c
    ${CMAKE_SOURCE_DIR}/memory/src/*.c
    ${CMAKE_SOURCE_DIR}/apps/*/src/*.c
)

file(GLOB_RECURSE ASM_SOURCES
    ${CMAKE_SOURCE_DIR}/drivers/asm/*.asm
)

# Compile .asm files manually
foreach(ASM_FILE ${ASM_SOURCES})
    file(RELATIVE_PATH ASM_REL_PATH ${CMAKE_SOURCE_DIR} ${ASM_FILE})
    string(REPLACE "/" "_" ASM_OBJ_NAME ${ASM_REL_PATH})
    string(REPLACE ".asm" "" ASM_OBJ_NAME ${ASM_OBJ_NAME})
    add_custom_command(
        OUTPUT ${OBJ_DIR}/${ASM_OBJ_NAME}.o
        COMMAND nasm -f elf ${ASM_FILE} -o ${OBJ_DIR}/${ASM_OBJ_NAME}.o
        DEPENDS ${ASM_FILE}
    )
    list(APPEND OBJECT_FILES ${OBJ_DIR}/${ASM_OBJ_NAME}.o)
endforeach()

# Compile .c files manually
foreach(C_FILE ${C_SOURCES})
    file(RELATIVE_PATH C_REL_PATH ${CMAKE_SOURCE_DIR} ${C_FILE})
    string(REPLACE "/" "_" C_OBJ_NAME ${C_REL_PATH})
    string(REPLACE ".c" "" C_OBJ_NAME ${C_OBJ_NAME})
    add_custom_command(
        OUTPUT ${OBJ_DIR}/${C_OBJ_NAME}.o
        COMMAND i386-elf-gcc ${CUSTOM_C_FLAGS} ${CUSTOM_INCLUDE_FLAGS} -c ${C_FILE} -o ${OBJ_DIR}/${C_OBJ_NAME}.o
        DEPENDS ${C_FILE}
    )
    list(APPEND OBJECT_FILES ${OBJ_DIR}/${C_OBJ_NAME}.o)
endforeach()

# Link kernel binary
add_custom_command(
    OUTPUT ${BUILD_DIR}/kernel.bin
    COMMAND i386-elf-ld -T ${CMAKE_SOURCE_DIR}/linker.ld -o ${BUILD_DIR}/kernel.bin -nostdlib ${OBJECT_FILES}
    DEPENDS ${OBJECT_FILES} ${CMAKE_SOURCE_DIR}/linker.ld
)

add_custom_target(kernel_bin ALL DEPENDS ${BUILD_DIR}/kernel.bin)

# Create ISO
add_custom_command(
    OUTPUT ${BUILD_DIR}/kernel.iso
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ISO_DIR}/boot/grub
    COMMAND ${CMAKE_COMMAND} -E copy ${BUILD_DIR}/kernel.bin ${ISO_DIR}/boot/kernel.bin
    COMMAND ${CMAKE_COMMAND} -E echo "set timeout=0" > ${ISO_DIR}/boot/grub/grub.cfg
    COMMAND ${CMAKE_COMMAND} -E echo "set default=0" >> ${ISO_DIR}/boot/grub/grub.cfg
    COMMAND ${CMAKE_COMMAND} -E echo "menuentry \"My OS\" { multiboot /boot/kernel.bin }" >> ${ISO_DIR}/boot/grub/grub.cfg
    COMMAND grub-mkrescue -o ${BUILD_DIR}/kernel.iso ${ISO_DIR}
    DEPENDS kernel_bin
)

add_custom_target(iso ALL DEPENDS ${BUILD_DIR}/kernel.iso)

# Run with QEMU
add_custom_target(run
    COMMAND qemu-system-i386 -cdrom ${BUILD_DIR}/kernel.iso -boot d -no-reboot -display gtk -audiodev pa,id=snd0,out.frequency=44100,out.channels=2,out.format=s16 -machine pcspk-audiodev=snd0
    DEPENDS iso
)

# Clean manually compiled .o files inside obj and iso directories
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${OBJ_DIR};${ISO_DIR}")
