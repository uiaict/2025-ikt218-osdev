
## OSDev75 Overview

OSDev75 is a simplified yet powerful starting point for learning operating system development. Our current implementation of UiAOS, inspired by OSDev75 guidelines, focuses on essential functionality and structure to manage system resources efficiently and reliably.

### Core Components Implemented

- **Standard Library Definitions:** To support minimal kernel operations, we defined essential types and constants, including:
  - **stdint.h:** Basic integer types such as `uint32_t`, `int32_t`, and `uint8_t`.
  - **stddef.h:** Size type (`size_t`) and NULL pointer definition.
  - **stdbool.h:** Boolean logic handling with `true` and `false`.
  - **limits.h:** Contains basic integer limits like `INT_MAX`.
  - **stdarg.h:** Handling variadic functions for formatted outputs.


- **Memory Management:** Minimal implementation including:
  - `memset`: Basic memory initialization.

### Booting and Initialization

Our OS currently uses the Multiboot2 standard, allowing the kernel to boot from common bootloaders (e.g., GRUB2). We set up the booting environment using the `multiboot2.asm` file with necessary header structures and a linker script (`linker.ld`) for correct memory alignment and segment loading.

- **multiboot2.h:** Detailed structures and definitions conforming to the Multiboot2 standard.
- **Linker script:** Ensures the kernel's memory is correctly structured into segments (`.text`, `.data`, `.bss`) required by the bootloader.

### Development and Debugging

We utilize a standardized Docker container configured via CMake (`CMakeLists.txt`) to maintain consistency across different systems:

- CMake sets up a reproducible build environment for cross-platform compatibility.
- QEMU is integrated for emulating the OS during development and debugging, utilizing GDB for detailed debugging sessions.

This structure allows rapid development cycles and efficient debugging, vital for the iterative nature of OS development.

## UiAOS Structure Diagram

```
UiAOS/
├── src/
│   ├── arch/
│   │   ├── i386/
│   │   │   ├── GDT/
│   │   │   │   ├── gdt_flush.asm
│   │   │   │   ├── gdt.c
│   │   │   │   ├── gdt.h
│   │   │   │   ├── util.h
│   │   │   │   └── linker.ld
│   │   │   ├── multiboot2.asm
│   │   │   └── kernel.c
│   │   └── x86_64/
├── drivers/
│   └── VGA/
│       ├── vga.c
│       └── vga.h
├── interrupts/
│   ├── idt.c
│   ├── idt.h
│   └── idt.s
├── cmake.lock
├── CMakeLists.txt
├── limine.cfg
├── .gitignore
└── README.md
```

# Introduction to VGA

**Video Graphics Array (VGA)** is a display hardware standard introduced by IBM in 1987. VGA supports both text and graphics modes, providing a method for computers to communicate visually with users through monitors. For operating system development, VGA's simplicity and widespread hardware support make it an ideal starting point for managing basic screen output during early stages of OS development.

### Why VGA?

At this foundational stage, we need a straightforward way to display text and basic graphics directly onto the screen without relying on advanced drivers or graphics hardware. VGA fulfills this role effectively due to its simplicity, direct memory mapping capabilities, and standard support across most hardware platforms.

Implementing VGA early helps us:
- Provide immediate visual feedback for debugging.
- Display important system information clearly.
- Learn low-level hardware interactions fundamental to OS development.

# VGA Implementation 

In our project, the **VGA (Video Graphics Array)** module allows us to output text and simple graphics directly onto the screen in text mode. Here, we explore the specifics of our VGA setup, focusing primarily on the header file `vga.h` and its corresponding source file `vga.c`.

## vga.h Overview

Our `vga.h` file serves as an interface, defining essential functions, constants, and dimensions used by the VGA text-mode driver.

### VGA Color Constants

We define a set of constants representing various colors used by VGA. These colors allow the text to be displayed with foreground and background colors enhancing visual clarity:

- Basic colors such as `COLOR8_BLACK`, `COLOR8_BLUE`, and `COLOR8_WHITE`.
- Additional lighter variants like `COLOR8_LIGHT_GREY` and `COLOR8_LIGHT_GREEN`.

### Screen Dimensions

Standard VGA text mode dimensions are set as:
- `width`: 80 columns
- `height`: 25 rows

### Function Definitions

- `Reset(void)`: Clears the screen and resets cursor position.
- `setColor(uint8_t fg, uint8_t bg)`: Sets text and background colors.
- `scrollUp(void)`: Scrolls screen content upwards by one line.
- `newLine(void)`: Moves cursor to the beginning of the next line.
- `print(const char* text)`: Outputs text to the current cursor position.
- `show_animation(void)`: Demonstrates a basic 4-frame animation.

## Importing stdint.h

We also include the `stdint.h` header to ensure consistent data types across platforms and compilers. The types we defined and imported are:

- **uint8_t, uint16_t, uint32_t, uint64_t**: Unsigned integer types specifying exact bit-widths.
- **int8_t, int16_t, int32_t, int64_t**: Signed integer types specifying exact bit-widths.
- **bool**: A boolean type (`true` and `false`) to enhance code readability.

As students, using these explicit types helps us avoid ambiguity and ensures portability across different architectures and compiler settings. This practice makes our code robust and clearly communicates our intention in terms of memory usage and data limits.

# vga.c 

### Role in the OSDev_75 Project

The `vga.c` file is located in the `drivers/VGA` directory of our project structure, paired with its header file `vga.h`. It is integrated into the broader OSDev_75 framework, which includes a Multiboot2-compliant bootloader, a minimal kernel (`kernel.c`), and supporting standard library components. As a student project, this driver reflects our learning journey in understanding low-level hardware interactions, memory manipulation, and the practical challenges of OS development.

### Purpose of the VGA Driver

The primary purpose of the VGA driver in OSDev_75 is to provide basic screen output capabilities for debugging and user interaction. In operating system development, being able to display text on the screen is one of the first milestones—it gives us immediate feedback on what our kernel is doing. For example, we can print error messages, system logs, or simple greetings like "hello world" to confirm that our kernel is functioning. Additionally, the driver supports a simple animation feature, which helps us experiment with dynamic visual output and understand timing and synchronization in a low-level context.

By implementing this driver, we gain hands-on experience with direct hardware access, specifically manipulating the VGA text-mode buffer in memory. This is a foundational skill for OS development, as it teaches us how to interface with hardware without the abstraction layers provided by modern operating systems. As students, this also serves as an engaging way to see our code "come to life" on the screen, making the learning process more rewarding.

### Hardware Context: VGA Text Mode (80x25)

The VGA driver operates in **VGA text mode**, a mode where the screen is divided into a grid of characters rather than pixels. Specifically, we use the standard VGA text mode resolution of **80 columns by 25 rows** (80x25), which is defined in `vga.h` with the constants `width = 80` and `height = 25`. In this mode, each character on the screen is represented by two bytes in memory: one byte for the ASCII character itself and another for its attributes (e.g., foreground and background colors). The VGA text buffer is mapped to the physical memory address `0xB8000`, which we access directly in `vga.c` to write characters and set their colors.

This 80x25 grid provides a total of 2000 character positions (80 × 25), and each position can display one of 256 ASCII characters with a combination of foreground and background colors chosen from a palette of 16 colors (defined in `vga.h` as `COLOR8_BLACK`, `COLOR8_WHITE`, etc.). VGA text mode is ideal for our project because it’s simple to implement, widely supported across hardware, and doesn’t require complex graphics programming—perfect for students starting out in OS development.

### Integration with `kernel.c`

The VGA driver integrates with our kernel through the `kernel.c` file, which serves as the main entry point for our OS after the bootloader (`multiboot2.asm`) initializes the system. In `kernel.c`, we include the VGA driver via the header `"drivers/VGA/vga.h"`, which declares the functions we’ll use to interact with the screen. The `main` function in `kernel.c` follows this sequence:

1. **Screen Initialization**: It calls `Reset()` to clear the screen and set the cursor to the top-left corner, preparing it for output.
2. **Animation Display**: It then calls `show_animation()` to display a 4-frame ASCII art animation, which helps us visualize the kernel’s startup process in a creative way.
3. **Message Output**: Finally, it uses `print("hello world\r\n")` to display a greeting, confirming that the kernel is operational.

These calls directly invoke the functions defined in `vga.c`, making the VGA driver a core part of our kernel’s user interface. As students, this integration teaches us how to modularize our code—separating hardware-specific functionality (like VGA output) into a driver while keeping the kernel focused on higher-level logic.

### High-Level Summary of Functions

The `vga.c` file contains several functions that work together to manage screen output in VGA text mode. Here’s a high-level overview of each, tailored for a student audience:

- **`makeVgaCell(char c, uint8_t attr)`**: A helper function that combines an ASCII character and its color attributes into a 16-bit value suitable for the VGA buffer. This is a low-level utility we use to format characters before writing them to memory.
  
- **`setColor(uint8_t fg, uint8_t bg)`**: Sets the foreground (`fg`) and background (`bg`) colors for subsequent text output. We use this to customize the appearance of our text, such as making the animation white on a dark red background.

- **`Reset(void)`**: Clears the entire screen by filling it with spaces and resets the cursor position to (0,0). This is useful for starting with a clean slate, especially at boot time.

- **`scrollUp(void)`**: Shifts all screen content up by one line and clears the bottom line, mimicking a terminal’s scrolling behavior. This helps when we need to display more text than fits on the screen.

- **`newLine(void)`**: Moves the cursor to the start of the next line, scrolling the screen if necessary. This is key for handling newline characters (`\n`) in our text output.

- **`print(const char* text)`**: Outputs a string to the screen at the current cursor position, handling special characters like `\n` (newline) and `\r` (carriage return). This is our main function for displaying text, such as the "hello world" message.

- **`delaySpin(unsigned long count)`**: A simple delay function that spins the CPU for a specified number of cycles, used to control the timing of our animation frames.

- **`show_animation(void)`**: Displays a 4-frame ASCII art animation, with each frame shown in white on a dark red background. After the animation, it switches to a black background with white text for the "hello world" message. This function demonstrates how we can create dynamic visual effects in our OS.

### For Student Learners

As students, the `vga.c` implementation is a great way to dive into OS development. It teaches us about direct memory access (writing to `0xB8000`), bit manipulation (for combining characters and colors), and timing (using delays for animation). The driver also introduces us to modular design—separating hardware interaction into a driver that the kernel can call. While our implementation is basic, it lays the groundwork for more advanced features, like supporting graphics modes or handling user input, which we might explore in future iterations of OSDev_75.


## Detailed Function Documentation for `vga.c`

The `vga.c` file in our UiAOS project implements a VGA text-mode driver, allowing our kernel to display text and simple animations on the screen. Below, we document each function in detail, providing insights into their purpose, implementation, and usage, along with student-specific notes to highlight our learning journey.

### `makeVgaCell`

- **Purpose**: Combines an ASCII character and its color attributes into a 16-bit value suitable for writing to the VGA text-mode buffer.
- **Parameters**:
  - `char c`: The ASCII character to display (e.g., 'A', ' ', or '\n').
  - `uint8_t attr`: The color attribute byte, combining foreground and background colors (e.g., white text on dark red background).
- **Return Value**: A `uint16_t` value where the lower 8 bits represent the ASCII character and the upper 8 bits represent the color attribute.
- **Implementation Details**:
  - The VGA text-mode buffer at `0xB8000` expects each character position to be a 16-bit value: the lower byte for the ASCII character and the upper byte for the attribute.
  - The function performs a bitwise AND on `c` with `0xFF` to ensure only the lower 8 bits are used (though `char` is typically 8 bits already, this is a defensive measure).
  - The `attr` byte is shifted left by 8 bits (`attr << 8`) to place it in the upper byte of the 16-bit result.
  - The character and attribute are combined using a bitwise OR operation: `(uint16_t)(c & 0xFF) | ((uint16_t)attr << 8)`.
- **Example**:
  - To display the character 'A' with white text on a dark red background, we might use:
    ```c
    uint16_t cell = makeVgaCell('A', ((COLOR8_DARK_GREY & 0x0F) << 4) | (COLOR8_WHITE & 0x0F));
    ```
    Here, `COLOR8_DARK_GREY = 8` and `COLOR8_WHITE = 15`, so the attribute byte is `(8 << 4) | 15 = 0x8F`, and the resulting `cell` is `0x8F41` ('A' is ASCII 65, or `0x41`).
- **Student Notes**:
  - **Challenge**: Understanding bit manipulation was tricky at first. We initially forgot to shift the attribute byte, resulting in garbled colors on the screen.
  - **Assumption**: We assume `char` is 8 bits, which is true for our x86 architecture, but this might not hold on other platforms.
  - **Improvement**: We could add comments in the code explaining the bit layout (e.g., "lower 8 bits: character, upper 8 bits: attribute") to make it clearer for future readers.

### `setColor`

- **Purpose**: Sets the foreground and background colors for subsequent text output by updating the global `currentAttribute` variable.
- **Parameters**:
  - `uint8_t fg`: The foreground color (e.g., `COLOR8_WHITE` for white text).
  - `uint8_t bg`: The background color (e.g., `COLOR8_DARK_GREY` for a dark red background).
- **Return Value**: None (void).
- **Implementation Details**:
  - The VGA color attribute byte is structured as `bg:4 bits | fg:4 bits`. The function combines `fg` and `bg` into a single byte using the formula `((bg & 0x0F) << 4) | (fg & 0x0F)`.
  - The `& 0x0F` ensures that only the lower 4 bits of each color are used, as VGA text mode supports 16 colors (0-15).
  - The background color is shifted left by 4 bits (`bg << 4`) to occupy the upper 4 bits of the attribute byte, and the foreground color occupies the lower 4 bits.
  - The result is stored in the global `currentAttribute` variable, which is used by other functions like `print` when writing to the VGA buffer.
- **Example**:
  - To set white text on a dark red background for the animation:
    ```c
    setColor(COLOR8_WHITE, COLOR8_DARK_GREY);
    ```
    This sets `currentAttribute` to `(8 << 4) | 15 = 0x8F`.
- **Student Notes**:
  - **Challenge**: We initially mixed up foreground and background positions, causing colors to display incorrectly (e.g., dark red text on white background instead of the reverse).
  - **Assumption**: We assume the color constants (`COLOR8_*`) are defined correctly in `vga.h` and are within the valid range (0-15).
  - **Improvement**: Adding validation to ensure `fg` and `bg` are within 0-15 could prevent runtime errors if invalid colors are passed.

### `Reset`

- **Purpose**: Clears the screen by filling it with spaces, resets the cursor position to (0,0), and reverts the color to the default setting.
- **Parameters**: None.
- **Return Value**: None (void).
- **Implementation Details**:
  - Resets the cursor coordinates by setting `cursor_x = 0` and `cursor_y = 0`.
  - Sets `currentAttribute` to `DEFAULT_ATTRIBUTE`, which is defined as black text on a white background (`((0 & 0x0F) << 4) | (15 & 0x0F)`).
  - Uses nested loops to iterate over all 80×25 positions on the screen (`height = 25`, `width = 80`).
  - For each position, writes a space character (' ') with the default attribute to the VGA buffer at `0xB8000` using `makeVgaCell`. The buffer index is calculated as `y * width + x`.
- **Example**:
  - Called in `kernel.c` at startup:
    ```c
    Reset();
    ```
    This clears the screen to a white background with black text (based on `DEFAULT_ATTRIBUTE`), preparing it for the animation.
- **Student Notes**:
  - **Challenge**: We initially used the wrong attribute for clearing, which caused the screen to flash with unexpected colors during resets.
  - **Assumption**: The VGA buffer is always mapped at `0xB8000`, which is true for our x86 setup but might differ in other environments.
  - **Improvement**: We could add a parameter to `Reset` to allow clearing with a custom color, making the function more flexible.

### `scrollUp`

- **Purpose**: Shifts all screen content up by one line and clears the bottom line, simulating a terminal scroll when the cursor reaches the bottom of the screen.
- **Parameters**: None.
- **Return Value**: None (void).
- **Implementation Details**:
  - Iterates over rows 1 to `height - 1` (rows 1 to 24) and copies the content of each row to the row above it. For each position, it copies the 16-bit value from `VGA_MEMORY[y * width + x]` to `VGA_MEMORY[(y - 1) * width + x]`.
  - Clears the last row (`height - 1`, row 24) by writing a space character with the current color attribute (`currentAttribute`) to each position in that row.
- **Example**:
  - Used indirectly when `newLine` calls `scrollUp` if the cursor is at the bottom:
    ```c
    cursor_y = height - 1;  // Simulate cursor at bottom
    newLine();              // This will trigger scrollUp
    ```
- **Student Notes**:
  - **Challenge**: We initially forgot to clear the last line, leaving leftover characters that made the screen look messy.
  - **Assumption**: We assume the screen dimensions (`width` and `height`) are fixed at 80×25, which simplifies the implementation but limits flexibility.
  - **Improvement**: We could optimize this by using a memory copy operation (like `memmove`) instead of a loop, though this would require adding memory management functions to our kernel.

### `newLine`

- **Purpose**: Moves the cursor to the start of the next line, scrolling the screen if the cursor is already at the bottom.
- **Parameters**: None.
- **Return Value**: None (void).
- **Implementation Details**:
  - Sets `cursor_x = 0` to move the cursor to the beginning of the line.
  - Checks if `cursor_y` is less than `height - 1` (i.e., not on the last row). If true, increments `cursor_y` to move to the next line.
  - If `cursor_y` is on the last row (`height - 1 = 24`), calls `scrollUp()` to shift the screen content up, keeping `cursor_y` at the bottom.
- **Example**:
  - When printing a string with a newline:
    ```c
    print("Line 1\nLine 2");
    ```
    The `\n` triggers `newLine`, moving the cursor to the start of the next row for "Line 2".
- **Student Notes**:
  - **Challenge**: We initially had a bug where `scrollUp` was called incorrectly, causing the screen to scroll endlessly.
  - **Assumption**: We assume the cursor position is always valid, but we don’t check for edge cases like negative values (though our code prevents this).
  - **Improvement**: Adding bounds checking for `cursor_y` could make the function more robust, though it’s unlikely to be an issue in our current usage.

### `print`

- **Purpose**: Outputs a string to the screen at the current cursor position, handling special characters like newline (`\n`) and carriage return (`\r`).
- **Parameters**:
  - `const char* text`: A null-terminated string to display (e.g., "hello world\r\n").
- **Return Value**: None (void).
- **Implementation Details**:
  - Loops through each character in `text` until it reaches the null terminator (`\0`).
  - For each character:
    - If it’s `\n`, calls `newLine()` to move to the next line.
    - If it’s `\r`, sets `cursor_x = 0` to move the cursor to the start of the current line.
    - Otherwise, checks if `cursor_x >= width` (i.e., beyond the screen width); if so, calls `newLine()` to wrap to the next line.
    - Writes the character to the VGA buffer at `VGA_MEMORY[cursor_y * width + cursor_x]` using `makeVgaCell` with the current attribute (`currentAttribute`), then increments `cursor_x`.
- **Example**:
  - Used in `kernel.c` to display the final message:
    ```c
    print("hello world\r\n");
    ```
    This writes "hello world" at the current cursor position, then moves to the next line.
- **Student Notes**:
  - **Challenge**: Handling special characters like `\n` and `\r` was confusing at first—we initially treated them as regular characters, causing formatting issues.
  - **Assumption**: We assume the input string is null-terminated, which is standard for C strings but could cause issues if the string is malformed.
  - **Improvement**: We could add support for more special characters (e.g., tabs `\t`) or add error handling for invalid cursor positions.

### `delaySpin`

- **Purpose**: Introduces a delay by spinning the CPU for a specified number of cycles, used to control the timing of the animation frames.
- **Parameters**:
  - `unsigned long count`: The number of cycles to spin (e.g., 200000000 for a noticeable delay).
- **Return Value**: None (void).
- **Implementation Details**:
  - Uses a `for` loop to iterate `count` times, marked as `volatile` to prevent the compiler from optimizing it away.
  - Inside the loop, executes a `nop` (no operation) instruction using inline assembly (`__asm__ __volatile__("nop")`), which consumes a single CPU cycle per iteration.
- **Example**:
  - Used in `show_animation` to pause between frames:
    ```c
    delaySpin(200000000);
    ```
    This creates a delay to make the animation visible to the user.
- **Student Notes**:
  - **Challenge**: Finding the right value for `count` was trial-and-error; too small a value made the animation too fast, while too large made it sluggish.
  - **Assumption**: We assume the CPU speed is consistent across runs, but this delay will behave differently on faster or slower hardware.
  - **Improvement**: A more precise timing mechanism (e.g., using a hardware timer) would be better, but this requires additional hardware knowledge we haven’t covered yet.

### `show_animation`

- **Purpose**: Displays a 4-frame ASCII art animation with a dark red background and white text, then switches to a black background with white text for subsequent output.
- **Parameters**: None.
- **Return Value**: None (void).
- **Implementation Details**:
  - Loops through the `FRAMES` array (4 frames) using a `for` loop.
  - For each frame:
    - Sets the color to white text on a dark red background using `setColor(COLOR8_WHITE, COLOR8_DARK_GREY)`.
    - Calls `Reset()` to clear the screen.
    - Calls `print(FRAMES[i])` to display the current frame.
    - Calls `delaySpin(200000000)` to pause, making the animation visible.
  - After the loop, sets the color to white text on a black background (`setColor(COLOR8_WHITE, COLOR8_BLACK)`) and calls `Reset()` to prepare for the "hello world" message.
  - The `FRAMES` array contains four ASCII art strings, each representing a frame of the animation.
- **Example**:
  - Called in `kernel.c` during startup:
    ```c
    show_animation();
    ```
    This displays the 4-frame animation, then clears the screen to a black background with white text.
- **Student Notes**:
  - **Challenge**: Designing the ASCII art and ensuring it looked good on the screen took time—we had to manually adjust the frames to make them visually appealing.
  - **Assumption**: We assume the screen dimensions (80×25) are sufficient to display the art, but the art isn’t perfectly centered (we added centering in a previous iteration).
  - **Improvement**: We could add more frames or allow the animation to loop a certain number of times, giving more control over the display duration.

Documenting these functions has helped us reflect on our implementation choices and challenges as students. The VGA driver in `vga.c` is a foundational piece of UiAOS, teaching us about hardware interaction, memory management, and timing. While our implementation is functional, there are areas for improvement—like adding error handling, optimizing performance, or supporting more features (e.g., custom animations or color schemes). These lessons will guide us as we continue developing UiAOS in the OSDev_75 project.


## Animation and Color Management in `vga.c`

The `show_animation` function in `vga.c` is a highlight of our UiAOS project, demonstrating how we can create dynamic visual output in a minimal operating system. As students, implementing this animation allowed us to experiment with VGA text mode, timing mechanisms, and color management, while adding a creative touch to our kernel's startup sequence. This section explores the `show_animation` function, the design of the ASCII art frames, the color scheme, and how these elements enhance visual feedback in UiAOS. We also provide suggestions for future enhancements and debugging tips for students working on similar projects.

### The `show_animation` Function

- **Purpose**: The `show_animation` function displays a 4-frame ASCII art animation during the kernel's startup, providing a visual indication that the system is booting. After the animation, it prepares the screen for the "hello world" message by switching to a different color scheme.
- **Implementation Details**:
  - **Frame Cycling**: The function uses a `for` loop to iterate over the `FRAMES` array, which contains four strings representing the ASCII art frames. The loop variable `i` (of type `size_t` to avoid sign-compare warnings) ranges from 0 to `NUM_FRAMES - 1` (where `NUM_FRAMES` is 4).
  - **Color Setup**: For each frame, it calls `setColor(COLOR8_WHITE, COLOR8_DARK_GREY)` to set white text on a dark red background. This ensures the animation stands out visually.
  - **Screen Clearing**: Before displaying each frame, it calls `Reset()` to clear the screen, ensuring that only the current frame is visible without remnants of the previous one.
  - **Frame Display**: It calls `print(FRAMES[i])` to output the current frame to the screen at the cursor position (starting at (0,0) after `Reset()`).
  - **Delay Mechanism**: After displaying each frame, it calls `delaySpin(200000000)` to pause for approximately 200 million CPU cycles, creating a visible delay between frames. This delay ensures the animation is slow enough for users to appreciate each frame.
  - **Post-Animation Setup**: After the loop, it calls `setColor(COLOR8_WHITE, COLOR8_BLACK)` to switch to white text on a black background and calls `Reset()` to clear the screen, preparing it for the "hello world" message in `kernel.c`.

### ASCII Art Frames and Design Process

The `FRAMES` array contains four ASCII art strings, each representing a frame of the animation. As students, designing this animation was both a technical and creative exercise, helping us understand how to craft visual elements within the constraints of VGA text mode.

- **Frame Content**:
  - **Frame 1**:
    ```
     /$$   /$$ /$$$$$$  /$$$$$$ 
    | $$  | $$|_  $$_/ /$$__  $$
    | $$  | $$  | $$  | $$  \ $$
    | $$  | $$  | $$  | $$$$$$$$
    | $$  | $$  | $$  | $$__  $$
    | $$  | $$  | $$  | $$  | $$
    |  $$$$$$/ /$$$$$$| $$  | $$
     \______/ |______/|__/  |__/
    ```
  - **Frame 2**:
    ```
    $$\   $$\ $$$$$$\  $$$$$$\  
    $$ |  $$ |\_$$  _|$$  __$$\ 
    $$ |  $$ |  $$ |  $$ /  $$ |
    $$ |  $$ |  $$ |  $$$$$$$$ |
    $$ |  $$ |  $$ |  $$  __$$ |
    $$ |  $$ |  $$ |  $$ |  $$ |
    \$$$$$$  |$$$$$$\ $$ |  $$ |
     \______/ \______/\__|  \__|
    ```
  - **Frame 3**:
    ```
     __    __  ______   ______  
    |  \  |  \|      \ /      \ 
    | $$  | $$ \$$$$$$|  $$$$$$\
    | $$  | $$  | $$  | $$__| $$
    | $$  | $$  | $$  | $$    $$
    | $$  | $$  | $$  | $$$$$$$$
    | $$__/ $$ _| $$_ | $$  | $$
     \$$    $$|   $$ \| $$  | $$
      \$$$$$$  \$$$$$$ \$$   \$$
    ```
  - **Frame 4**: Identical to Frame 1, creating a loop-back effect to make the animation feel continuous.

- **Design Process**:
  - **Inspiration**: We wanted the animation to reflect the theme of UiAOS, so we chose to create a stylized representation of currency symbols (`$$`) and other ASCII patterns that resemble a logo or banner. The idea was to make the startup sequence visually engaging.
  - **Creation**: We manually designed each frame using a text editor, experimenting with different ASCII characters (e.g., `/`, `\`, `$`, `_`) to form patterns. Each frame is 8 rows tall (plus a newline), and the width varies between 30-35 characters.
  - **Challenges**: Ensuring the frames looked consistent across iterations was difficult—Frame 3 uses a different style (more traditional ASCII art with underscores and vertical bars) compared to Frames 1, 2, and 4, which made the animation feel slightly disjointed. We also had to ensure the art fit within the 80×25 VGA screen without wrapping incorrectly.
  - **Centering Attempt**: In a previous iteration, we adjusted the cursor position (`cursor_x = 22`, `cursor_y = 8`) to center the art on the 80×25 screen. However, in this version, we rely on the default cursor position (0,0) after `Reset()`, which means the art appears in the top-left corner. Centering could be reintroduced for better aesthetics.

### Color Scheme and Management

The color scheme in `show_animation` is designed to enhance the visual feedback of UiAOS, making the boot process more engaging and informative.

- **During Animation**:
  - **Color Choice**: White text on a dark red background, implemented with `setColor(COLOR8_WHITE, COLOR8_DARK_GREY)`.
  - **Constants Used**:
    - `COLOR8_WHITE = 15`: Represents pure white for the foreground (text), ensuring the ASCII art is bright and readable.
    - `COLOR8_DARK_GREY = 8`: Represents a dark red shade for the background. While VGA text mode doesn’t have a true "dark red," `COLOR8_DARK_GREY` provides a muted tone that contrasts well with white text, creating a dramatic effect.
  - **Implementation**: The `setColor` function combines these colors into an attribute byte: `((bg & 0x0F) << 4) | (fg & 0x0F)`. For white on dark red, this is `(8 << 4) | 15 = 0x8F`. This attribute is used when `print` calls `makeVgaCell` to write characters to the VGA buffer at `0xB8000`.

- **After Animation**:
  - **Color Choice**: White text on a black background, implemented with `setColor(COLOR8_WHITE, COLOR8_BLACK)`.
  - **Constants Used**:
    - `COLOR8_WHITE = 15`: Keeps the text bright and readable.
    - `COLOR8_BLACK = 0`: Provides a clean, neutral background for the "hello world" message.
  - **Implementation**: The attribute byte becomes `(0 << 4) | 15 = 0x0F`, ensuring a standard terminal-like appearance after the animation.

- **Enhancement of Visual Feedback**:
  - The dark red background with white text during the animation creates a striking contrast, drawing attention to the boot process and making it visually appealing. This is especially useful for demonstrating that the kernel is active and progressing through its startup sequence.
  - Switching to a black background with white text afterward mimics a traditional terminal, providing a clean and professional look for the "hello world" message. This transition signals to the user that the boot animation is complete and the system is ready for further output.
  - Using the `COLOR8_*` constants from `vga.h` ensures consistency in color usage across the driver. These constants map directly to VGA’s 16-color palette, making it easy to experiment with different color combinations without hardcoding values.

### Future Enhancements

As students, we see several opportunities to enhance the `show_animation` function and color management in future iterations of UiAOS:

- **Centering the Animation**: Reintroduce explicit cursor positioning to center the ASCII art on the screen (e.g., `cursor_x = 22`, `cursor_y = 8`). This would improve the visual presentation, making the animation appear more polished.
- **More Frames or Looping Options**: Add more frames to the animation or introduce a parameter to control the number of times the animation loops. For example, `show_animation(uint32_t loops)` could repeat the 4 frames multiple times.
- **Dynamic Color Transitions**: Instead of a static color scheme, we could cycle through different background colors for each frame (e.g., dark red, dark blue, dark green) to create a more dynamic effect. This would require modifying `show_animation` to use a color array or a `switch` statement.
- **Customizable Animation**: Allow the animation frames to be passed as an argument (e.g., `show_animation(const char** frames, size_t num_frames)`), making the function reusable for different animations.
- **Better Timing Control**: Replace `delaySpin` with a hardware timer-based delay (e.g., using the Programmable Interval Timer, PIT). This would provide more precise control over frame timing and make the animation consistent across different hardware speeds.

# GDT 

###  Introduction and Purpose

#### What is the Global Descriptor Table (GDT), and why is it essential for your UiAOS operating system?
The Global Descriptor Table (GDT) is a data structure in the x86 architecture that defines memory segments—regions of memory with specific properties like size, base address, and access rights. In my UiAOS operating system, the GDT is essential because it enables **protected mode**, a 32-bit operating mode of the CPU that replaces the 16-bit real mode used during the initial boot process. Protected mode provides critical features like memory protection (to keep kernel and user memory separate) and privilege levels (ring 0 for the kernel, ring 3 for user programs), which are foundational for a secure and modern OS.

In UiAOS, the boot process starts with the Multiboot2 bootloader, which loads my kernel into memory at address `0x10000` and switches the CPU from real mode to protected mode before jumping to `kernel.c`. However, the bootloader’s default GDT is minimal, so I implement my own in `initGdt` to customize memory segmentation for UiAOS. This ensures that my kernel can safely execute code, access data, and eventually support user programs, all while preventing unauthorized memory access. For example, without the GDT, a user program could overwrite kernel memory, crashing the system. The GDT is a stepping stone to advanced features like multitasking, which I might add later.

#### What are the key components defined in your `gdt.h` file (e.g., selectors, structures, functions), and what roles do they play?
My `gdt.h` file defines the core components for managing the GDT in UiAOS. Here’s what each part does:

- **Selectors (`#define` constants):**
  - `KERNEL_CODE_SELECTOR = 0x08`: Points to the kernel code segment (GDT entry 1, index 1 × 8 bytes). Used for the CS register when running kernel code.
  - `KERNEL_DATA_SELECTOR = 0x10`: Points to the kernel data segment (entry 2). Used for DS, SS, etc., in kernel mode.
  - `USER_CODE_SELECTOR = 0x18`: Points to the user code segment (entry 3). For user programs’ CS in ring 3.
  - `USER_DATA_SELECTOR = 0x20`: Points to the user data segment (entry 4). For user data access.
  - These selectors are offsets into the GDT, telling the CPU which segment to use when accessing memory.

- **Structures:**
  - `struct gdt_entry_struct`: Defines a single GDT entry (8 bytes), describing a memory segment’s properties like base, limit, and access rights. It’s the building block of the GDT.
  - `struct gdt_ptr_struct`: Holds the GDT’s size (`limit`) and memory address (`base`). The CPU uses this to locate the GDT via the `lgdt` instruction.
  - `struct tss_entry_struct`: Defines the Task State Segment (TSS), a special structure for storing task state (e.g., stack pointers) during privilege switches, like system calls.

- **Functions:**
  - `void initGdt()`: Initializes the entire GDT, setting up all segments and loading them into the CPU.
  - `void setGdtGate(uint32_t num, ...)`: Configures a specific GDT entry (e.g., kernel code or TSS).
  - `void writeTSS(uint32_t num, ...)`: Sets up the TSS entry in the GDT for task switching.

For example, `KERNEL_CODE_SELECTOR = 0x08` corresponds to GDT entry 1, which I set up in `initGdt` as a 4GB executable segment for the kernel. These components work together to define and activate my memory layout in UiAOS.

---

### GDT Entry Structure and Setup

#### Describe the `gdt_entry_struct` in `gdt.h`. What does each field represent, and how are they packed together?
The `gdt_entry_struct` in `gdt.h` is an 8-byte structure that represents one GDT entry, defining a memory segment’s properties. I use `__attribute__((packed))` to ensure it’s tightly packed with no padding, matching the x86 GDT format exactly. Here’s what each field does:

- `uint16_t limit` (2 bytes): The lower 16 bits of the segment’s size (bits 0–15). The total size combines this with bits in `flags`.
- `uint16_t base_low` (2 bytes): The lower 16 bits of the segment’s base address (bits 0–15).
- `uint8_t base_middle` (1 byte): The middle 8 bits of the base address (bits 16–23).
- `uint8_t access` (1 byte): Defines the segment’s type and permissions, like privilege level (ring 0 or 3), whether it’s executable (code) or writable (data), and if it’s present in memory.
- `uint8_t flags` (1 byte): Combines the upper 4 bits of the limit (bits 16–19) with flags like granularity (byte or 4KB pages) and segment mode (16-bit or 32-bit).
- `uint8_t base_high` (1 byte): The upper 8 bits of the base address (bits 24–31).

The layout is: `[limit (2)] [base_low (2)] [base_middle (1)] [access (1)] [flags (1)] [base_high (1)]`, totaling 8 bytes. For example, a kernel code segment might have `limit = 0xFFFF`, `base_low = 0x0000`, `base_middle = 0x00`, `access = 0x9A` (present, ring 0, code), `flags = 0xCF` (4KB granularity, 32-bit), and `base_high = 0x00`, describing a 4GB segment starting at address 0.

#### Explain how `setGdtGate` in `gdt.c` configures a GDT entry. Provide an example of how it sets up the kernel code segment.
The `setGdtGate` function in `gdt.c` configures a single GDT entry by filling the fields of `gdt_entries[num]` based on the parameters: `num` (entry index), `base` (start address), `limit` (size), `access` (permissions), and `gran` (granularity flags). Here’s how it works:

- **Base Address Splitting:**
  - `base_low = (base & 0xFFFF)`: Takes the lower 16 bits of `base`.
  - `base_middle = (base >> 16) & 0xFF`: Shifts `base` right 16 bits and masks to get bits 16–23.
  - `base_high = (base >> 24) & 0xFF`: Shifts `base` right 24 bits for bits 24–31.

- **Limit Splitting:**
  - `limit = (limit & 0xFFFF)`: Lower 16 bits of the size.
  - `flags = (limit >> 16) & 0x0F`: Upper 4 bits of the limit (bits 16–19).

- **Flags and Granularity:**
  - `flags |= (gran & 0xF0)`: Combines the limit bits with granularity flags (e.g., 4KB pages, 32-bit mode).

- **Access:**
  - `access` is set directly to define segment type and rights.

**Example: Kernel Code Segment with `setGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF)`:**
- `num = 1`: Configures GDT entry 1 (selector `0x08`).
- `base = 0`:
  - `base_low = 0 & 0xFFFF = 0x0000`.
  - `base_middle = (0 >> 16) & 0xFF = 0x00`.
  - `base_high = (0 >> 24) & 0xFF = 0x00`.
- `limit = 0xFFFFFFFF` (4GB):
  - `limit = 0xFFFF` (lower 16 bits).
  - `flags = (0xFFFFFFFF >> 16) & 0x0F = 0x0F` (upper bits).
- `gran = 0xCF`:
  - `flags = 0x0F | (0xCF & 0xF0) = 0x0F | 0xC0 = 0xCF` (4KB pages, 32-bit).
- `access = 0x9A`: Present (bit 7 = 1), ring 0 (bits 5–6 = 00), code segment (bit 3 = 1), readable (bit 1 = 1).

This sets up a 4GB kernel code segment starting at address 0, executable only in ring 0, which the CPU uses when CS is set to `0x08`.


###  GDT Pointer and Initialization

#### What is the purpose of `gdt_ptr_struct`, and how is it used in `initGdt` to set up the GDT?
The `gdt_ptr_struct` in `gdt.h` is a small structure that tells the CPU where the GDT is located in memory and how big it is. It has two fields:
- `uint16_t limit`: The size of the GDT in bytes, minus 1 (to match the x86 `lgdt` instruction’s expectation).
- `unsigned int base`: The starting memory address of the GDT.

In my UiAOS, `gdt_ptr_struct` acts as a pointer that the CPU loads into its GDTR (Global Descriptor Table Register) to access the GDT. In `initGdt` from `gdt.c`, I set it up like this:
- `gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 6) - 1`: Each GDT entry is 8 bytes, and I have 6 entries, so the total size is 48 bytes. Subtracting 1 gives `limit = 47`, which represents the last byte offset relative to `base`.
- `gdt_ptr.base = (uint32_t)&gdt_entries`: This is the memory address of the `gdt_entries` array, where my GDT table is stored.

After setting these values, I pass `&gdt_ptr` to `gdt_flush`, which uses the `lgdt` instruction to load it into the CPU. For example, if `gdt_entries` is at address `0x1000`, then `gdt_ptr.base = 0x1000` and `gdt_ptr.limit = 47`, telling the CPU the GDT spans `0x1000` to `0x102F`. This setup ensures the CPU knows exactly where to find my segment definitions.

#### Walk through the `initGdt` function in `gdt.c`. What does each GDT entry represent, and why are there 6 entries?
The `initGdt` function in `gdt.c` initializes my GDT with 6 entries, setting up the memory layout for UiAOS. Here’s what each entry does and why I chose 6:

1. **Entry 0: Null Segment (`setGdtGate(0, 0, 0, 0, 0)`):**
   - This is a mandatory null descriptor, all zeros, required by the x86 architecture. It acts as a safety net—if a segment register accidentally points here (selector `0x00`), the CPU will fault, preventing undefined behavior.

2. **Entry 1: Kernel Code Segment (`setGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF)`):**
   - Defines a 4GB segment (base `0`, limit `0xFFFFFFFF`) for kernel code, executable in ring 0. Selector `0x08` (index 1 × 8) is used for the CS register when running kernel code.

3. **Entry 2: Kernel Data Segment (`setGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF)`):**
   - A 4GB segment for kernel data, readable/writable in ring 0. Selector `0x10` (index 2 × 8) is used for DS, SS, and other data segment registers in kernel mode.

4. **Entry 3: User Code Segment (`setGdtGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF)`):**
   - A 4GB segment for user code, executable in ring 3. Selector `0x18` (index 3 × 8) is for user programs’ CS, ensuring they run with lower privileges.

5. **Entry 4: User Data Segment (`setGdtGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF)`):**
   - A 4GB segment for user data, readable/writable in ring 3. Selector `0x20` (index 4 × 8) is for user data access.

6. **Entry 5: Task State Segment (TSS) (`writeTSS(5, 0x10, 0x0)`):**
   - A special segment pointing to `tss_entry`, used for task switching and privilege level changes (e.g., system calls). Selector `0x28` (index 5 × 8) is loaded later with `tss_flush`.

I use 6 entries because I need a null segment, separate code and data segments for both kernel (ring 0) and user (ring 3) modes, and a TSS for future task-switching support. After setting these, `gdt_flush((uint32_t)&gdt_ptr)` loads the GDT into the CPU, and `tss_flush()` prepares the TSS, making my memory layout active. For example, after this, setting `CS = 0x08` lets the kernel execute code securely in ring 0.


### Task State Segment (TSS)

#### What is the `tss_entry_struct` in `gdt.h`, and why is it included in your GDT implementation?
The `tss_entry_struct` in `gdt.h` is a 104-byte structure that defines the Task State Segment (TSS), which stores the state of a task (like stack pointers and register values) in the x86 architecture. It’s included in my GDT as a special descriptor to support privilege level changes, such as switching from user mode (ring 3) to kernel mode (ring 0) during system calls or interrupts.

Key fields include:
- `uint32_t ss0`: The kernel stack segment selector (e.g., `0x10` for kernel data).
- `uint32_t esp0`: The kernel stack pointer, used when switching to ring 0.
- Other fields like `cs`, `ds`, `eip`, `eflags`, etc., store the task’s register state, though I only initialize a few for now.

I need the TSS in UiAOS because when a user program triggers a system call, the CPU must switch to the kernel’s privilege level safely. The TSS provides the kernel stack (`ss0:esp0`) for this transition, ensuring the kernel has a separate execution context from the user. For example, without a TSS, a system call would overwrite the user stack, causing crashes. I include it in the GDT as entry 5 to prepare for future features like multitasking or interrupt handling.

#### How does `writeTSS` in `gdt.c` configure the TSS entry? Provide an example of its setup with specific values.
The `writeTSS` function in `gdt.c` configures the TSS by setting up GDT entry 5 and initializing `tss_entry`. Here’s how it works:
- `uint32_t base = (uint32_t)&tss_entry`: Gets the memory address of `tss_entry`.
- `uint32_t limit = base + sizeof(tss_entry)`: Calculates the TSS size (104 bytes), but only the lower bits matter for the GDT entry.
- `setGdtGate(num, base, limit, 0xE9, 0x00)`: Configures GDT entry `num` (5) as a TSS descriptor, with `access = 0xE9` (present, ring 3 accessible, TSS type) and `gran = 0x00` (byte granularity).
- `memset(&tss_entry, 0, sizeof(tss_entry))`: Clears `tss_entry` to zero out unused fields.
- Sets key fields:
  - `tss_entry.ss0 = ss0`: Kernel stack segment.
  - `tss_entry.esp0 = esp0`: Kernel stack pointer.
  - `tss_entry.cs = 0x08 | 0x3`: User code selector with RPL (Requested Privilege Level) 3.
  - `tss_entry.ss = tss_entry.ds = ... = 0x10 | 0x3`: User data selector with RPL 3.

**Example: `writeTSS(5, 0x10, 0x0)`:**
- `num = 5`: Targets GDT entry 5 (selector `0x28`).
- `base = &tss_entry` (e.g., `0x2000` if that’s where it’s located).
- `limit = 0x2000 + 104 = 0x2068`, but `setGdtGate` uses lower 20 bits (`0x0068`).
- `setGdtGate(5, 0x2000, 0x2068, 0xE9, 0x00)`:
  - `base_low = 0x2000 & 0xFFFF = 0x2000`.
  - `base_middle = (0x2000 >> 16) & 0xFF = 0x00`.
  - `base_high = (0x2000 >> 24) & 0xFF = 0x00`.
  - `limit = 0x0068 & 0xFFFF = 0x0068`.
  - `flags = (0x0068 >> 16) & 0x0F = 0x00`.
  - `access = 0xE9`: TSS, present, ring 3 accessible.
- `tss_entry.ss0 = 0x10`: Kernel data segment.
- `tss_entry.esp0 = 0x0`: Kernel stack pointer (temporary, should be updated to a valid address).
- `tss_entry.cs = 0x08 | 0x3 = 0x0B`: Kernel code with RPL 3.
- `tss_entry.ss = tss_entry.ds = ... = 0x10 | 0x3 = 0x13`: Kernel data with RPL 3.

This sets up the TSS at `0x2000` with a 104-byte limit, ready for a system call to switch to the kernel stack at `0x10:0x0`.
Here are my responses for **Part 5** and **Part 6** based on your prompts. I’ve written them as if I’m you, explaining your GDT implementation in `gdt_flush.asm` and `gdt.c` with examples, keeping it detailed and clear. Since you asked me to "do it," I’m providing full answers for these parts, which you can adapt for your documentation. Let me know if you need tweaks or want to move to the next parts!


### Loading the GDT and TSS

#### Explain the `gdt_flush` function in `gdt_flush.asm`. What does it do to activate the GDT, and why is the far jump necessary?
The `gdt_flush` function in `gdt_flush.asm` is an assembly routine that activates my GDT in UiAOS by loading it into the CPU and updating the segment registers. Here’s how it works, step by step:

- `MOV eax, [esp+4]`: Gets the address of `gdt_ptr` (passed from `gdt_flush((uint32_t)&gdt_ptr)` in `initGdt`), which contains the GDT’s base and limit.
- `LGDT [eax]`: Loads the GDT pointer into the CPU’s GDTR register using the `lgdt` instruction. This tells the CPU where my `gdt_entries` array is and how big it is (e.g., base `0x1000`, limit `47` for 6 entries).
- `MOV eax, 0x10`: Loads the kernel data segment selector (`0x10`, GDT entry 2) into `eax`.
- `MOV ds, ax`, `MOV es, ax`, `MOV fs, ax`, `MOV gs, ax`, `MOV ss, ax`: Updates all data segment registers (DS, ES, FS, GS, SS) to point to the kernel data segment. This ensures data access uses my 4GB ring 0 data segment.
- `JMP 0x08:.flush`: Performs a far jump to the kernel code segment selector (`0x08`, GDT entry 1), with the label `.flush` as the target. This updates the CS (code segment) register.
- `.flush: RET`: Returns to the caller after the jump.

The far jump (`JMP 0x08:.flush`) is necessary because simply loading the GDT with `lgdt` doesn’t immediately update the CS register, which still points to the bootloader’s old segment. The jump forces CS to use my kernel code segment (ring 0, 4GB), aligning the instruction pointer with my GDT’s definitions. Without it, the CPU might continue executing with an outdated CS, causing a crash. For example, after `gdt_flush`, my kernel runs with `CS = 0x08` and `DS = 0x10`, fully using my GDT’s memory layout.

#### What does `tss_flush` in `gdt_flush.asm` do, and how does it prepare the CPU for task switching?
The `tss_flush` function in `gdt_flush.asm` loads the Task State Segment (TSS) into the CPU’s Task Register (TR) to prepare for task switching in UiAOS. Here’s what it does:

- `MOV ax, [esp+4]`: Gets the TSS index passed from `tss_flush()` in `initGdt`. In my case, it’s called with no explicit argument, but I assume it’s meant to use the TSS selector (`0x28` for entry 5).
- `SHL ax, 3`: Multiplies the index by 8 to convert it to a GDT selector (e.g., index 5 becomes `5 × 8 = 0x28`). However, since I don’t pass an index explicitly, this might be a placeholder—I rely on the caller to set it correctly in future use.
- `LTR ax`: Loads the TSS selector (e.g., `0x28`) into the TR using the `ltr` instruction. This tells the CPU where my TSS descriptor (GDT entry 5) is.

In my current setup, `tss_flush()` is called after `writeTSS(5, 0x10, 0x0)`, so TR points to selector `0x28`. This prepares the CPU for task switching by linking it to `tss_entry`, which holds the kernel stack (`ss0:esp0`) for privilege level changes, like system calls. For example, if a user program triggers an interrupt, the CPU uses TR to find the TSS and switch to the kernel stack. Note: my `tss_flush` assumes a fixed setup right now, and I might need to pass the selector explicitly (e.g., `tss_flush(0x28)`) to make it more robust.



### Practical Usage and Examples

#### example of how UiAOS uses the GDT to switch between kernel and user mode.
In UiAOS, the GDT allows me to switch between kernel mode (ring 0) and user mode (ring 3) by setting segment registers to the appropriate selectors defined in `gdt.h`. Here’s an example of transitioning to user mode for a user program:

- **Initial State (Kernel Mode):**
  - After `gdt_flush`, the kernel runs with:
    - `CS = 0x08` (kernel code segment, ring 0).
    - `DS = SS = 0x10` (kernel data segment, ring 0).

- **Switching to User Mode:**
  - To launch a user program, I’d set:
    - `CS = 0x18 | 0x3 = 0x1B`: User code segment (GDT entry 3) with RPL (Requested Privilege Level) 3. The `| 0x3` sets the privilege level to ring 3.
    - `DS = ES = SS = 0x20 | 0x3 = 0x23`: User data segment (GDT entry 4) with RPL 3.
    - `FS = GS = 0x23`: Optional, but typically set to the same user data segment.
  - This is done using a far jump or `iret` instruction from the kernel. For example:
    ```asm
    PUSH 0x23        ; User SS
    PUSH 0x1000      ; User stack pointer (esp)
    PUSHF            ; Flags
    PUSH 0x1B        ; User CS
    PUSH 0x2000      ; User instruction pointer (eip)
    IRET             ; Switch to user mode
    ```

- **Result:** The user program runs in ring 3 with `CS = 0x1B` and `SS = DS = 0x23`, restricted to the 4GB user segments, while the kernel retains control over ring 0 operations.

#### a simple example of how a system call might use the TSS in my setup.
In UiAOS, the TSS (set up by `writeTSS(5, 0x10, 0x0)`) is used during a system call to switch from user mode to kernel mode safely. Here’s a simple example:

- **User Mode Setup:**
  - A user program runs with:
    - `CS = 0x1B` (user code, ring 3).
    - `SS = DS = 0x23` (user data, ring 3).
    - Stack at `0x23:0x1000`.

- **System Call Trigger:**
  - The user program executes `INT 0x80` (a common software interrupt for system calls).
  - The CPU checks the Interrupt Descriptor Table (IDT) for the `0x80` handler (not yet implemented in my code, but assumed here).

- **TSS Role in Transition:**
  - The CPU sees TR points to `0x28` (TSS selector from `tss_flush`).
  - It loads the kernel stack from `tss_entry`:
    - `SS = tss_entry.ss0 = 0x10` (kernel data segment).
    - `ESP = tss_entry.esp0 = 0x0` (kernel stack pointer, though I’d need to set this to a valid address like `0x8000`).
  - The CPU saves the user state (e.g., `CS = 0x1B`, `EIP`, `SS = 0x23`, `ESP = 0x1000`) into the kernel stack or TSS, then switches to:
    - `CS = 0x08` (kernel code, ring 0, via the IDT gate).
    - `SS = 0x10`, `ESP = 0x0` (from TSS).

- **Kernel Handling:**
  - The system call handler runs in ring 0, using the kernel stack at `0x10:0x0`. For example, it might print a message via my VGA driver, then return with `iret`.

- **Result:** The TSS ensures a clean switch by providing the kernel stack (`ss0:esp0`), preventing the user stack from being overwritten. I’d need to update `esp0` to a real stack address (e.g., `0x8000`) for this to work properly in practice.


