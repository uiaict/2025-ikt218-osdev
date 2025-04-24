# Introduction

Interrupts are signals from a device, such as a keyboard or a hard drive, to the CPU, telling it to immediately stop whatever it is currently doing and do something else. For example, a keyboard controller can send an interrupt when a character key was pressed. Then the OS can display the character on screen immediately, even if the CPU was doing something completely unrelated before, and return to what it was doing afterwards.

## Hardware Interrupt

### Type 1: IRQ 

    [action]                 [small memory]                                               
You press a button [A] --> keyboard's internal buffer stores scan code for [A] --> keyboard controller transfers scan code to I/O 
                       [attention please 1 for keybord]            [Posten]                                       [okay i am here]
port 0x60 --> keyboard controller activates IRQ line [1] --> PIC receives IRQ signal and forwards to CPU --> CPU suspends current 
                                                                                             [MAP]
task and saves state to stack --> CPU determines interrupt number from IRQ line --> CPU uses interrupt number to index into IDT --> 
                                                                               [worker]
CPU finds address of keyboard handler in IDT --> CPU jumps to handler --> handler reads scan code from I/O port 0x60 --> handler converts scan code to character and places in OS keyboard buffer --> handler signals EOI --> CPU restores state from stack --> CPU returns to previous task

When you press a key [A], the keyboard's internal microcontroller detects this. The keyboard itself contains a small hardware buffer(or small memory) (typically 16-128 bytes) implemented as a circular buffer or FIFO (First-In-First-Out) queue. This allows the keyboard to handle multiple rapid keypresses even if the CPU is momentarily busy.
The keyboard controller converts your keypress [A] into a scan code (a numerical value that identifies which key was pressed). This scan code is placed into the keyboard's internal buffer
The keyboard controller then transfers the scan code from its internal buffer to a dedicated I/O port on the computer (commonly port 0x60 on x86 systems). This I/O port acts as a single-byte buffer between the keyboard and the CPU.
After placing the scan code in the I/O port, the keyboard controller activates its assigned IRQ line (typically IRQ 1 for keyboards). This is just a signal wire carrying a binary "attention needed" signal.
The Programmable Interrupt Controller (PIC) receives this signal on the IRQ line. The PIC prioritizes this interrupt among any others that might be occurring simultaneously and forwards it to the CPU via the interrupt pin.
When the CPU receives the interrupt signal, it:
Completes its current instruction
Saves its current execution state (registers, flags, etc.) onto the system stack
Determines which interrupt occurred (based on which IRQ line was activated)
Uses this interrupt number as an index into the Interrupt Descriptor Table (IDT)
The IDT is a data structure in memory that contains entries mapping interrupt numbers to the memory addresses of their handling routines. Each entry in the IDT (known as a "gate descriptor" on x86) is typically 8 bytes and contains:
The memory address of the handler routine
Privilege level information
Type of gate (interrupt, trap, etc.)
The CPU jumps to the memory address specified in the IDT entry. This keyboard interrupt handler code knows that it should:
Read from I/O port 0x60 to retrieve the scan code that was placed there
Convert the scan code to an ASCII character or other representation
Place this character in the keyboard input buffer (a software buffer maintained by the OS)
Possibly signal any processes waiting for keyboard input
The CPU jumps to the memory address specified in the IDT entry. This keyboard interrupt handler code knows that it should:
Read from I/O port 0x60 to retrieve the scan code that was placed there
Convert the scan code to an ASCII character or other representation
Place this character in the keyboard input buffer (a software buffer maintained by the OS)
Possibly signal any processes waiting for keyboard input
After processing, the handler executes an End-of-Interrupt (EOI) instruction to inform the PIC that it can send more interrupts. The CPU then pops the saved state from the stack and returns to what it was doing before.
### Type 2: MSI (Message-Signaled Interrupts)
Imagine you're downloading a file or streaming a video - data packets arrive at your computer's network card, and the system needs to process them efficiently without constantly checking if new data has arrived.
When a packet of data arrives from the internet (like part of a webpage you're loading or a chunk of a file you're downloading), your network card needs to:

Receive the data
Store it somewhere
Tell the CPU "Hey, I've got new data for you to process!"

Network card receives data packet --> card stores packet in its onboard buffer --> card transfers packet to system memory via DMA --> card writes an MSI message to a special memory address (containing device ID and event type) --> memory controller detects this special write --> memory controller forwards interrupt to CPU --> CPU suspends current task and saves state to stack --> CPU determines interrupt details from the MSI message data --> CPU finds address of network handler in IDT --> CPU jumps to handler --> handler processes the network data already in memory --> handler updates descriptor rings to acknowledge processing --> CPU restores state from stack --> CPU returns to previous task

The key data structures involved are:
1. Network card's onboard packet buffer (typically a ring buffer)
2. System memory DMA buffer region (where packet data is transferred)
3. MSI address/data registers (special memory locations for interrupt signaling)
4. CPU's system stack (for saving state)
5. Interrupt Descriptor Table (array of handler addresses)
6. Descriptor rings or queues (tracking which packets have been processed)
MSI uses memory writes rather than dedicated physical interrupt lines. Let me explain how this works in detail, focusing on the data structures involved:

1. **Device Data Buffer**: When a network card receives a data packet from the network, it first stores this packet in its own onboard buffer memory. This buffer is typically organized as a circular buffer or a linked list of buffer descriptors, allowing the network card to queue up multiple packets.

2. **DMA Transfer**: The network card then uses Direct Memory Access (DMA) to transfer the packet data from its onboard buffer to a pre-allocated region in the main system memory. This region is often organized as a ring buffer or descriptor queue, shared between the device and the driver.

3. **Interrupt Generation**: After transferring the data, the network card needs to notify the CPU. Instead of using a physical IRQ line, with MSI the card performs a special write operation to a specific memory address that was allocated during device initialization. This write contains:
   - A message data value (typically 16 bits) that indicates the specific event type
   - This data is written to a pre-defined memory address that's monitored by the system

4. **Memory Controller Detection**: The system's memory controller detects this special write operation to the MSI address range. Since these are special addresses configured for interrupt signaling, the memory controller recognizes this isn't a normal memory write.

5. **CPU Notification**: The memory controller forwards this interrupt notification to the CPU. Unlike traditional interrupts that come through the interrupt pin, this comes through the memory subsystem.

6. **CPU Interrupt Processing**: When the CPU receives this notification, it:
   - Completes its current instruction
   - Saves its current execution state (registers, flags, etc.) onto the system stack
   - Examines the memory address and data value that was written to determine which device and what type of event occurred
   - Uses this information to determine which interrupt handler to call

7. **IDT Lookup**: The CPU uses the information from the MSI to find the appropriate handler in the Interrupt Descriptor Table (IDT), just as with traditional interrupts.

8. **Handler Execution**: The CPU jumps to the handler's memory address. This network interrupt handler knows that:
   - The network data has already been placed in system memory via DMA
   - It can find the location of this data through descriptor rings or other data structures maintained by the driver
   - It needs to process this data and pass it up to higher networking layers

9. **Return From Interrupt**: After processing, the handler informs the hardware that the interrupt has been handled (though the mechanism differs from traditional EOI), and the CPU returns to its previous task.

The main advantage of MSI is that it eliminates the need for dedicated interrupt lines, allowing more devices to generate unique interrupts without sharing lines. It's also more efficient in many cases, as the interrupt mechanism uses the same path (the memory bus) that the data already travels on.
## Comparison: Why Network Cards Use MSI and Keyboards Use IRQ via PIC
Here’s a detailed and concise section for your documentation focusing on the "Comparison: Why network cards use MSI and keyboards use IRQ via PIC." This section explains the rationale behind the choice of interrupt mechanisms based on device characteristics, efficiency, and hardware design, tailored to your OSDev_75 context.

---

### Comparison: Why Network Cards Use MSI and Keyboards Use IRQ via PIC

The choice of interrupt mechanism—Message-Signaled Interrupts (MSI) for network cards and Interrupt Requests (IRQ) via the Programmable Interrupt Controller (PIC) for keyboards—reflects the differing needs of these devices in terms of data volume, frequency of interrupts, and hardware architecture.

- **Network Cards Use MSI:**
  - **High Data Volume and Frequency:** Network cards handle large, continuous data streams (e.g., video streaming or file downloads), generating frequent interrupts as packets arrive. MSI, supported over the PCIe (PCI Express) bus, uses memory writes to signal interrupts, allowing multiple unique interrupts without the limitation of the PIC’s 16 IRQ lines.
  - **Efficiency with PCIe:** PCIe, a high-speed bus standard, integrates MSI natively, enabling devices to write interrupt messages (including device ID and event type) directly to a special memory address. This leverages the memory bus, reducing latency and aligning with the high-bandwidth requirements of network data transfer via Direct Memory Access (DMA).
  - **Scalability:** MSI eliminates the need for shared interrupt lines, making it ideal for modern systems with many high-throughput devices. It allows the CPU to identify the interrupt source directly from the MSI message, avoiding PIC overhead.

- **Keyboards Use IRQ via PIC:**
  - **Low Data Volume and Infrequent Interrupts:** Keyboards generate sparse interrupts (e.g., one per keypress), producing minimal data (1 byte per scan code). The legacy PIC system, with its 16 IRQ lines, is sufficient for such low-frequency events, making it a cost-effective choice.
  - **Cost-Effectiveness and Simplicity:** Keyboards often use legacy interfaces (e.g., PS/2) or USB with legacy emulation, designed for IRQ-based interrupts managed by the PIC. This avoids the complexity and cost of PCIe or MSI hardware, which is overkill for a device with such limited interrupt needs.
  - **Efficiency for Low Frequency:** The PIC’s interrupt prioritization ensures reliable handling of infrequent keyboard events without overwhelming the system. Sending an EOI after processing keeps the interrupt pipeline efficient for devices with minimal data throughput.

**PCI Express Context:**
PCIe, a successor to the older PCI standard, supports MSI and is optimized for high-speed devices like network cards. Its architecture allows memory-mapped interrupts, which suit the data-intensive nature of networking. In contrast, keyboards typically rely on the PIC’s IRQ lines due to their legacy design and low interrupt rate, aligning with the OSDev_75’s use of PIC for IRQ 1 in the keyboard handler.

### Example: Webcam vs. TV Channel Camera Setup

**My PC Default Webcam Setup:**
My PC’s default webcam, likely an older model built into my Razer laptop, is of low quality—probably less than 720p, perhaps around 640x480 or 320x240 resolution, typical for older laptops. It connects via USB and uses an IRQ-based interrupt mechanism, likely emulated through the USB host controller. When I use it for video calls, it captures low-resolution video (e.g., 15-30 fps at a bitrate of maybe 1-5 Mbps), compresses it (possibly using MJPEG), and sends it to my computer. The USB controller generates interrupts to notify the CPU of new frames, which are processed by the OS and displayed in an application like Skype. This setup works because the data rate is minimal, fitting within USB 2.0’s bandwidth (up to 480 Mbps) and the limited capabilities of the PIC or USB interrupt handling, making it efficient for basic personal use on an old laptop.

**TV Channel Camera Setup:**
In contrast, a TV channel using an 8K or 4K camera for live streaming, such as during a sports broadcast, cannot rely on a setup like my low-quality webcam. These professional cameras produce massive amounts of data—uncompressed 4K at 30 fps can require around 400 Mbps, and 8K at 60 fps might exceed 3.2 Gbps. Such high data rates and the need for real-time reliability make IRQ via PIC or USB-based setups impractical due to their limited bandwidth and overhead. Instead, TV channels use professional-grade interfaces like SDI (Serial Digital Interface) or IP-based connections. SDI supports uncompressed video, ensuring high quality over long distances (up to 100 meters), ideal for studio environments where a camera might connect to a production switcher. Alternatively, IP-based connections (e.g., using NDI or SMPTE 2110 standards) enable network streaming, supporting remote production workflows. These setups use dedicated hardware for video processing, often bypassing traditional CPU interrupt mechanisms like IRQ or MSI, and focus on hardware-level synchronization.

**My Idea on Efficiency Reasoning (Why Keyboards Don’t Use MSI):**
This comparison also ties into my earlier question about why keyboards don’t use MSI, which I think comes down to efficiency reasoning. Keyboards, like my webcam, produce very little data—just one byte per keypress—and generate interrupts infrequently. Using IRQ via PIC is efficient for this because the PIC’s simple prioritization and EOI mechanism handle low-frequency events without overhead. MSI, on the other hand, is designed for high-throughput devices like network cards, where frequent interrupts and large data transfers benefit from memory-based signaling over PCIe. For a keyboard, setting up MSI would be overkill, adding unnecessary complexity and resource use for minimal gain, since the PIC’s IRQ system already meets the need efficiently. Similarly, my webcam uses IRQ over USB because its low data rate doesn’t justify a more complex mechanism like MSI, reinforcing that efficiency drives these design choices.



## Software Interrupts

Software interrupts are different from hardware interrupts because they're triggered by software instructions rather than external devices. Let me explain using the "Hello, World!" example:

Imagine that we have a compiled program helloWorld.a that comes from this C source code:

```c
#include <stdio.h>  // Standard I/O library

int main() {
    // printf is a standard library function, but inside it there's code that makes a system call for printing things
    printf("Hello, world!\n"); 

    return 0;  // Returning 0 indicates successful execution.
}
```

When we run this program, here's what happens with software interrupts:

1. The program starts executing and reaches the `printf()` function call.

2. The `printf()` function in the C standard library prepares the string "Hello, world!\n" and any formatting needed.

3. Inside `printf()`, after preparing the data, the library code needs to ask the operating system to actually display text on the screen. It does this through a system call, which is implemented as a software interrupt.

4. To make this system call, the program:
   - Places the system call number (identifying that this is a "write" operation) in a specific CPU register
   - Places other parameters (like the string pointer and length) in other registers
   - Executes a special instruction like `int 0x80` (on older Linux/x86) or `syscall` (on newer systems)

5. This special instruction triggers a software interrupt, causing the CPU to:
   - Save the current program's state to the stack
   - Switch from user mode to kernel mode (gaining higher privileges)
   - Jump to the operating system's system call handler code

6. The OS's system call handler:
   - Examines the registers to determine which system call was requested (in this case, "write")
   - Validates the parameters to ensure they're safe
   - Performs the requested operation by writing to the console device driver
   - Places any return values in registers
   - Switches back from kernel mode to user mode
   - Returns control to the program right after the system call instruction

7. The `printf()` function receives the return value (how many characters were printed) and eventually returns to `main()`.

8. The program continues executing until it returns from `main()` and terminates.

The flow looks like this:
Program executes printf() --> printf() prepares string data --> printf() sets up system call parameters in registers --> printf() executes the syscall instruction --> CPU switches to kernel mode --> OS system call handler runs --> OS writes text to console --> OS switches back to user mode --> Program continues execution --> Text appears on screen

The key data structures involved are:
1. The program's stack (to save state during the interrupt)
2. CPU registers (to pass system call number and parameters)
3. The system call table (mapping system call numbers to handler functions)
4. The console device driver's buffers (where the text is staged before display)
5. The display buffer (where characters are mapped to screen positions)

Software interrupts allow regular programs to safely request privileged operations from the operating system, maintaining system security while providing standard services to applications.

## Exception Interrupts

Exception interrupts (traps or faults) are automatically generated by the CPU itself when it encounters an exceptional condition during program execution. Unlike hardware or software interrupts, these aren't explicitly requested by either devices or program code - they're triggered by the CPU detecting an error or special condition.

examples :

1. **Divide-by-zero exception**: When a program attempts to divide a number by zero
2. **Page fault**: When a program tries to access memory that isn't currently mapped in the virtual memory system
3. **Invalid opcode**: When the CPU encounters an instruction it can't recognize or execute

The flow for exception handling is similar to other interrupts:
- CPU detects the exceptional condition during instruction execution
- CPU identifies the exception type and generates an internal interrupt
- CPU saves the current execution state to the stack
- CPU looks up the appropriate handler in the IDT
- The exception handler runs to address the condition (which might involve terminating the program, allocating memory, or other corrective actions)
- If recoverable, the CPU returns control to the program (possibly at a different address)


Below is an improved version of your documentation for the sections "Deep Down on the PIC's," "From the CPU's Perspective," and "From the OS's Perspective." I’ve enhanced clarity, added technical depth where appropriate, integrated the assembly code for completeness, addressed the suggested improvements, and ensured a cohesive flow. The improvements focus on refining the narrative, adding precision, and tying the examples more tightly to the perspectives, while keeping your personal context (Razer laptop, 'H' and 'I' keypresses) intact.

---

### Deep Down on the PIC's

#### Overview of the Interrupt Flow Before and After the PIC

The interrupt journey begins with a hardware device signaling its need for attention. For example, when I press a key on my keyboard—say, 'H' followed by 'I'—the keyboard’s microcontroller detects each keypress, translates it into a scan code (e.g., 0x23 for 'H', 0x17 for 'I'), and stores it in its internal buffer (a FIFO queue, typically 16-128 bytes). The keyboard controller then transfers the scan code to the I/O port 0x60, a 1-byte buffer accessible to the CPU, and activates its assigned Interrupt Request (IRQ) line—usually IRQ 1 for keyboards—sending a binary "attention needed" signal.

The Programmable Interrupt Controller (PIC) steps in as the mediator, managing this signal across its 16 lines, handled by two cascaded 8259A chips: a master (IRQs 0-7) and a slave (IRQs 8-15), with the slave’s output connected to the master’s IRQ 2 line. Upon receiving IRQ 1, the PIC evaluates its priority within a fixed hierarchy (0, 1, 2, 8, 9, …, 7), asserts the INTR pin to alert the CPU, and provides an interrupt vector during the acknowledge cycle. By default, IRQ 0-7 map to 08h-0Fh and IRQ 8-15 to 70h-77h, but the OS typically remaps them (e.g., 20h-27h for master, 28h-2Fh for slave) to avoid conflicts with CPU-reserved interrupts (0-31).

After the PIC, the CPU takes control. It saves its state (registers, flags) to the stack, indexes the Interrupt Descriptor Table (IDT) with the vector, and jumps to the handler. The handler processes the interrupt—reading the scan code from 0x60, converting it to a character, and updating the OS’s keyboard buffer—before sending an End-of-Interrupt (EOI) to the PIC (via `out 0x20, 0x20` for master, or `out 0xA0, 0x20` followed by `out 0x20, 0x20` for slave if needed). The CPU then restores its state and resumes its prior task, completing the cycle.

#### Example: Handling Two Keypresses ('H' and 'I') with Assembly

Consider an example where I type 'H' and 'I' on my keyboard, triggering two IRQ 1 interrupts. Let’s explore this with a simplified assembly implementation of the handler.

**Scenario:**
- **Keypress 1: 'H'** (Scan code 0x23)
- **Keypress 2: 'I'** (Scan code 0x17)
- The keyboard controller sends these to port 0x60, raising IRQ 1 each time.

**PIC’s Role:**
The master PIC receives each IRQ 1, prioritizes it (above IRQ 3, below IRQ 0), and signals the CPU with INTR, providing vector 21h (assuming remapping to 20h-27h). It awaits the EOI after each event.

**Assembly Implementation:**
```nasm
; Keyboard Interrupt Handler for IRQ 1 (remapped to INT 21h)
global keyboard_handler
keyboard_handler:
    cli                    ; Disable interrupts to prevent nesting
    pushad                 ; Save all general-purpose registers

    ; Read scan code from port 0x60
    in al, 0x60            ; AL = scan code (e.g., 0x23 for 'H', 0x17 for 'I')
    mov bx, scan_code_table; BX points to scan code to ASCII table
    xlatb                  ; AL = ASCII character (e.g., 'H' or 'I')
    cmp al, 0              ; Check if valid
    je .end                ; Skip if invalid

    ; Write to VGA buffer at 0xB8000 (incrementing offset)
    mov edi, [vga_offset]  ; EDI = current VGA buffer offset
    mov ah, 0x07           ; Attribute: light gray on black
    mov [edi], ax          ; Write character and attribute
    add edi, 2             ; Move to next position
    mov [vga_offset], edi  ; Update offset

.end:
    ; Send EOI to PIC
    mov al, 0x20           ; EOI command
    out 0x20, al           ; Send to master PIC

    popad                  ; Restore registers
    sti                    ; Re-enable interrupts
    iret                   ; Return from interrupt

; Data section
section .data
vga_offset dd 0xB8000    ; Start of VGA text buffer
scan_code_table:
    db 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0
    db 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a'
    db 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 0, 0, 0, 'z', 'x', 'c'
    db 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '  ; Illustrative; 'H' (0x23), 'I' (0x17) need exact mapping
```

**Step-by-Step Process:**
1. **First Keypress ('H'):**
   - Scan code 0x23 triggers IRQ 1.
   - The PIC signals vector 21h.
   - The CPU executes `keyboard_handler`, converts 0x23 to 'H', and writes it to 0xB8000.
   - EOI is sent, and the PIC resets.

2. **Second Keypress ('I'):**
   - Scan code 0x17 triggers IRQ 1 again.
   - The PIC signals vector 21h.
   - The CPU writes 'I' to the next position.
   - Another EOI is sent.

**PIC’s Deep Insight:**
The PIC views each keypress as a unique IRQ 1 event, managing the master-slave cascade and reserving IRQ 2 for the slave. Remapping to 20h-27h avoids CPU exception conflicts, showcasing its adaptability. For my Razer laptop, it efficiently handles my low-quality webcam and keyboard, though its 16-line limit marks it as a legacy system compared to MSI.


### From the CPU's Perspective

From the CPU's perspective, the execution landscape is a rhythmic flow of instructions, periodically disrupted by interrupt signals from the PIC, demanding swift action. The CPU cycles through fetching, decoding, and executing, maintaining its state in registers and flags, until the INTR pin pulses, signaling a device like my keyboard or webcam requires attention.

Upon detecting an interrupt, the CPU finishes its current instruction for atomicity, then initiates an interrupt acknowledge cycle. It queries the PIC, which returns a vector (e.g., 21h for remapped IRQ 1). The CPU automatically saves its state—pushing EFLAGS, CS, and EIP onto the stack—and, in protected mode, switches to kernel mode (privilege level 0) based on the IDT’s gate descriptor, potentially loading a new stack. This transition, driven by the descriptor’s privilege and segment details, ensures secure handler execution.

Using the vector, the CPU indexes the IDT to fetch the handler address. For my 'H' keypress (IRQ 1, vector 21h), it jumps to `keyboard_handler`, executing instructions to read scan code 0x23 from port 0x60, convert it to 'H', and write it to the VGA buffer at 0xB8000. The CPU executes this blindly, focused on instruction fidelity, unaware of the keyboard’s role.

Post-handler—after the EOI (`out 0x20, 0x20`) is issued—the CPU restores its state with `pop` operations and `iret`, returning to the interrupted task. A subsequent 'I' keypress repeats this, with the CPU managing each interrupt sequentially. This disciplined process ensures responsiveness for my Razer laptop’s low-frequency devices, though it hinges on the PIC’s 16-line constraint, hinting at future scalability challenges.


### From the OS's Perspective

From the operating system’s perspective, interrupt handling is the backbone of system responsiveness, transforming PIC and CPU actions into a cohesive user experience. The OS acts as the architect, initializing and managing the interrupt framework to support devices like my keyboard and webcam.

During boot, the OS reprograms the PIC to remap IRQs, avoiding CPU-reserved interrupts (0-31). It configures the master PIC for 20h-27h (e.g., IRQ 1 to 21h) and the slave for 28h-2Fh, then populates the IDT with gate descriptors—linking vector 21h to `keyboard_handler`—and sets privilege levels for secure kernel-mode transitions. This setup prepares the system for hardware events.

When I press 'H' and 'I', the OS interprets these as IRQ 1 events. The PIC signals the CPU, which invokes the handler via the IDT. The OS’s keyboard driver reads scan codes 0x23 and 0x17 from port 0x60, converts them to 'H' and 'I' using a lookup table, buffers them, and renders 'HI' to the VGA buffer via the driver, reflecting my input. The OS ensures the handler sends an EOI to the PIC, clearing each interrupt for the next event, and manages the display update.

The OS also safeguards against issues, validating interrupt sources to resolve shared IRQ conflicts and handling exceptions (e.g., halting on a divide-by-zero by logging the error and terminating the process). For my Razer laptop, the OS leverages the PIC’s efficiency for its low-data, low-frequency webcam and keyboard. Yet, it acknowledges the PIC’s 16-IRQ limit as a bottleneck for modern devices like network cards, noting MSI as a potential future upgrade—though its complexity delays adoption. From the OS’s lens, the PIC is a dependable legacy foundation, adaptable but poised for evolution.



### Overview Summary: IDT, PIC, and IRQ – What I Learned So Far

**What I’ve Learned About Interrupts:**
Working on this OS project has taught me a lot about how interrupts handle communication between hardware and software. It all starts when a device like a keyboard or mouse needs attention. For example, when I press 'H' and 'I', the keyboard sends scan codes (0x23 and 0x17) to port 0x60, triggering an interrupt. The PIC, CPU, and OS work together to process this, and I’ve figured out how the Interrupt Descriptor Table (IDT), Programmable Interrupt Controller (PIC), and Interrupt Requests (IRQs) fit into it.

**About the IDT:**
The IDT is like a big lookup table that the CPU uses to find interrupt handlers. It has 256 entries, covering vectors 0 to 255, and its location is stored in the IDTR register, loaded with the LIDT instruction. In 32-bit mode, the IDTR is 48 bits (32-bit offset, 16-bit size), and each entry is 8 bytes, making the table 2048 bytes total. In 64-bit mode, it’s 80 bits (64-bit offset, 16-bit size), with 16-byte entries, totaling 4096 bytes. Each entry is a gate descriptor with an offset (split into parts), a segment selector pointing to the GDT, a gate type (like 0xE for 32-bit Interrupt Gate), DPL for privilege, and a P bit for validity. Vectors 0-31 are for CPU exceptions (e.g., divide-by-zero at 0, page fault at 14), 32-47 are for our PIC IRQs after remapping (e.g., IRQ 1 at 21h), and 48-255 are for software interrupts like system calls at 80h. The IDT gives us flexibility to handle hardware and software events as our OS grows.

**About the PIC:**
The PIC is the middleman that manages hardware interrupts for us. It uses two 8259A chips—a master for IRQs 0-7 and a slave for IRQs 8-15, cascaded through the master’s IRQ 2 line—giving us 16 IRQs total. When a device like the keyboard triggers IRQ 1, the PIC prioritizes it (e.g., 0, 1, 2, 8, 9, …, 7), asserts the INTR pin to tell the CPU, and sends a vector during the acknowledge cycle. By default, IRQs map to 08h-0Fh and 70h-77h, but our OS remaps them to 20h-2Fh to avoid exception conflicts. After the CPU handles the interrupt, it sends an EOI (e.g., `out 0x20, 0x20`) to the PIC, letting it clear the line for the next interrupt. The PIC is perfect for our simple setup with keyboards and mice, but its 16-IRQ limit shows it’s a legacy system.

**About IRQs:**
IRQs are the signals devices use to get the CPU’s attention. With the PIC, we have 16 IRQs (0-15), like IRQ 1 for the keyboard or IRQ 12 for the mouse. Each IRQ gets mapped to an IDT vector after remapping—IRQ 1 to 21h, IRQ 12 to 2Ch, and so on. The process starts when a device sends its signal, the PIC handles it, and the CPU runs the right handler. I learned that different devices use different methods: keyboards and mice use PIC IRQs for their low data (one byte per keypress), network cards use MSI over PCIe for high data rates, and TV cameras use SDI or IP for live streaming, not USB like a webcam. This shows why our PIC setup works for us but might not scale for everything.

**Putting It All Together:**
The interrupt flow is a team effort. A device triggers an IRQ, the PIC prioritizes and sends a vector (e.g., 21h for 'H' and 'I'), the CPU uses the IDT to run the handler (writing 'HI' to 0xB8000), and the OS makes sure it all works, sending an EOI back to the PIC. I’ve learned how the IDT’s 256 entries cover exceptions, our IRQs, and software interrupts, while the PIC manages our 16 IRQs efficiently. It’s cool to see how this setup handles our basic devices, but I also see that for more hardware or software (like printing or files), we might need something like MSI or APIC in the future.

# moving to code 

## **idt.c**

```c
#include "stdint.h"
#include "../GDT/util.h"
#include "../VGA/vga.h"
#include "idt.h"
#include "string.h"
```
The code begins by including essential headers for integer types, utility I/O functions (like `outPortB`), VGA output, IDT declarations, and string operations. These provide the basic building blocks needed to set up and manage interrupt descriptors.

```c
struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);
```
Here, we define a global array `idt_entries` with 256 entries and a single `idt_ptr` that holds the base address and size of that array. The external function `idt_flush` is an assembly routine that will load our new IDT using the `LIDT` instruction.

```c
void initIdt() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

    // PIC initialization for IRQs
    outPortB(0x20, 0x11);
    outPortB(0xA0, 0x11);
    outPortB(0x21, 0x20);
    outPortB(0xA1, 0x28);
    outPortB(0x21, 0x04);
    outPortB(0xA1, 0x02);
    outPortB(0x21, 0x01);
    outPortB(0xA1, 0x01);
    outPortB(0x21, 0x0);
    outPortB(0xA1, 0x0);
    
    // ISRs 0-31 (exceptions)
    setIdtGate(0, (uint32_t)isr0, 0x08, 0x8E);
    setIdtGate(1, (uint32_t)isr1, 0x08, 0x8E);
    ...
    setIdtGate(31, (uint32_t)isr31, 0x08, 0x8E);

    // IRQs 0-15 (mapped to IDT 32-47)
    setIdtGate(32, (uint32_t)irq0, 0x08, 0x8E);
    ...
    setIdtGate(47, (uint32_t)irq15, 0x08, 0x8E);

    // Syscall and custom interrupt
    setIdtGate(128, (uint32_t)isr128, 0x08, 0xEE); // syscalls at DPL 3
    setIdtGate(177, (uint32_t)isr177, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
```
`initIdt()` sets up the IDT and remaps the PIC. First, it configures the IDT pointer by setting the limit to the size of our `idt_entries` array minus one and assigning its base. Then it clears that array with `memset`. It proceeds to reprogram the master and slave PIC chips so hardware interrupts align with vectors 32 to 47 instead of their original vectors. Afterward, it configures each entry with `setIdtGate`, linking exceptions (0–31), hardware IRQs (32–47), a syscall interrupt (128), and a custom interrupt (177). Finally, it calls `idt_flush` to tell the processor to load this new table.

```c
void setIdtGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}
```
`setIdtGate` fills a single entry in the table. It uses the lower and upper 16 bits of `base` to store the address of the interrupt or exception handler, sets the code segment selector `sel`, clears the reserved byte, and adds flags that define its privilege level and gate type.

```c
char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    ...
    "Reserved"
};
```
Here we keep strings that describe each CPU exception in a human-readable form. These are used inside `isr_handler` to print messages based on the exception number.

```c
void isr_handler(struct InterruptRegisters* regs) {
    print("ISR: ");
    if (regs->int_no < 32) {
        print(exception_messages[regs->int_no]);
        print("\n");
        if (regs->int_no == 0 || regs->int_no == 8 || regs->int_no == 14) {
            print("Fatal Exception! System Halted\n");
            for(;;);
        }
    } else if (regs->int_no == 128) {
        print("System Call (int 0x80)\n");
    } else if (regs->int_no == 177) {
        print("Custom Interrupt 177\n");
    } else {
        print("Unknown Interrupt ");
        char num[10];
        itoa(regs->int_no, num, 10);
        print(num);
        print("\n");
    }
}
```
`isr_handler` is the routine that handles exceptions (and certain software interrupts). It checks the interrupt number in `regs->int_no` and prints the appropriate message if it matches a known exception. It halts for fatal ones. It also displays whether it was a system call (0x80) or a custom interrupt (177). Everything else is labeled as unknown.

```c
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};
```
`irq_routines` is a simple array of function pointers for the 16 possible IRQs. If a driver needs to hook into an IRQ (like the keyboard on IRQ 1), it can register a callback here.

```c
void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}
```
These two let external drivers install or uninstall custom handlers for a given IRQ line.

```c
void irq_handler(struct InterruptRegisters* regs) {
    void (*handler)(struct InterruptRegisters *regs);
    handler = irq_routines[regs->int_no - 32];

    if (handler) {
        handler(regs);
    } // else left commented out to prevent spam

    if (regs->int_no >= 40) {
        outPortB(0xA0, 0x20); // EOI for slave PIC
    }
    outPortB(0x20, 0x20); // EOI for master PIC
}
```
`irq_handler` is called when a hardware interrupt happens. It checks if someone installed a function in `irq_routines` for that specific IRQ. If so, it calls that function. Finally, it sends an End of Interrupt signal to the PIC so that new interrupts can be delivered.

---

## **idt.h**

```c
#ifndef IDT_H
#define IDT_H

#include "stdint.h"
#include "types.h"  
```
The header begins with guards to prevent multiple inclusions. It then brings in standard integer types and our custom `types.h` for `InterruptRegisters`.

```c
struct idt_entry_struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
```
These two structs define how each IDT gate looks and how the CPU loads the entire IDT. `__attribute__((packed))` makes sure the compiler doesn’t insert any extra padding.

```c
void initIdt();
void setIdtGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void isr_handler(struct InterruptRegisters* regs);
void irq_handler(struct InterruptRegisters* regs);
void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *regs));
void irq_uninstall_handler(int irq);
```
These are our main interface functions. `initIdt` sets up everything. `setIdtGate` configures one gate in the table. `isr_handler` and `irq_handler` are the core routines for handling exceptions and IRQs. `irq_install_handler` and `irq_uninstall_handler` let devices hook or unhook their own interrupt routines.

```c
extern void idt_flush(uint32_t);
extern void isr0();
extern void isr1();
...
extern void isr31();
extern void isr128();
extern void isr177();
extern void irq0();
extern void irq1();
...
extern void irq15();
```
These `extern` declarations connect us to the assembly stubs. Each one is the low-level entry point for its respective interrupt. `idt_flush` is also an assembly routine that loads the new IDT pointer with `LIDT`.

```c
#endif


## **idt.asm**

```nasm
global idt_flush
idt_flush:
    MOV eax, [esp+4]
    LIDT [eax]
    STI
    RET
```
This is the routine that updates the CPU’s interrupt descriptor table register (IDTR) with the address and size of our newly built table. It reads the pointer from the stack (where C code passed it) and executes `LIDT [eax]`. After that, it uses `STI` to enable interrupts. Finally, it returns to the caller in C.

```nasm
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG 0
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro IRQ 2
    global irq%1
    irq%1:
        CLI
        PUSH LONG 0
        PUSH LONG %2
        JMP irq_common_stub
%endmacro
```
These three macros generate the assembly stubs that correspond to different types of interrupts.  
- **ISR_NOERRCODE** creates a label like `isr0` or `isr1` for exceptions where the CPU does not push an error code automatically. It still pushes `0` as a placeholder, followed by the interrupt number.  
- **ISR_ERRCODE** sets up a label for exceptions that do have a real error code (like Invalid TSS).  
- **IRQ** is used for hardware interrupts (IRQs) that map to a certain vector. It pushes zero and then the vector number, since hardware interrupts also don’t have a CPU-pushed error code.

After pushing the right values on the stack, each macro jumps to either `isr_common_stub` or `irq_common_stub`.

```nasm
ISR_NOERRCODE 0
ISR_NOERRCODE 1
...
ISR_NOERRCODE 31
ISR_NOERRCODE 128
ISR_NOERRCODE 177

IRQ 0, 32
IRQ 1, 33
...
IRQ 15, 47
```
Here we invoke the macros for all CPU exceptions (0 through 31), two custom exceptions (128 and 177), and for hardware IRQ lines (0 through 15). Each one creates a unique assembly entry label, like `isr0` or `irq1`, which the IDT code in C references with `setIdtGate(...)`.

```nasm
extern isr_handler
isr_common_stub:
    pusha
    mov eax, ds
    PUSH eax
    MOV eax, cr2
    PUSH eax

    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp
    CALL isr_handler

    POP eax         ; Pop esp
    POP eax         ; Pop CR2
    POP ebx         ; Pop DS
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx

    POPA
    ADD esp, 8      ; remove err_code, int_no
    STI
    IRET
```
`isr_common_stub` is the shared entry point for CPU exceptions. It saves registers (`pusha`), pushes DS and CR2, and switches to the kernel data segment (0x10). It then calls the C function `isr_handler`, which decides what to do based on the interrupt number. After the call returns, it restores the old DS, pops registers back, re-enables interrupts, and executes `IRET`.

```nasm
extern irq_handler
irq_common_stub:
    pusha
    mov eax, ds
    PUSH eax
    MOV eax, cr2
    PUSH eax

    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp
    CALL irq_handler

    POP eax         ; Pop esp
    POP eax         ; Pop CR2
    POP ebx         ; Pop DS
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx

    POPA
    ADD esp, 8
    STI
    IRET
```
`irq_common_stub` is almost identical but calls `irq_handler` instead. Hardware interrupts come through here, and this is where we eventually signal End Of Interrupt (EOI) to the PIC in the C code. By the time we return, the stack and data segments are back to their original state before the interrupt occurred.

In this file, everything is assembled into one place, from loading a new IDT with `idt_flush` to creating entry stubs for all exceptions and IRQs. It’s the low-level glue that makes the C-based handlers (`isr_handler` and `irq_handler`) work.



## Example 1: Trigger a Custom Interrupt in Your Own Test Function

Imagine you created a new interrupt vector, say 0x55, by writing a line in `idt.asm` like:

```
ISR_NOERRCODE 85   ; 85 decimal = 0x55 in hexadecimal
```

You also called `setIdtGate(85, (uint32_t)isr85, 0x08, 0x8E);` in `initIdt()`. To test that interrupt, you could do something like this in a separate C function:

```c
void testCustomInterrupt(void) {
    // Force the CPU to call the new interrupt vector 0x55
    __asm__ __volatile__("int $0x55");

    // If everything is working, the CPU jumps to isr85 in assembly,
    // which jumps to your shared stub, which should eventually
    // invoke whatever custom logic you have for vector 0x55.
}
```

This confirms your IDT setup for interrupt 0x55 is working, since manually triggering it sends control through the assembly stub and into your higher-level code.

---

## Example 2: Hooking an IRQ for a Hypothetical Hardware Device

Suppose a device on **IRQ 7** needs a special routine to handle its interrupts. You have something like `irq7` in `idt.asm` pointing to your `irq_common_stub`. In C, you might declare:

```c
void myDeviceIrqHandler(/* parameters for CPU state if desired */) {
    // Respond to the device here. For example, read a status register,
    // acknowledge the hardware interrupt, etc.
    // The rest of your system code would handle EOI (End Of Interrupt).
}

void initMyDevice(void) {
    // Suppose you have a function that installs a handler
    // into your table of IRQ callbacks.
    irq_install_handler(7, &myDeviceIrqHandler);
}
```

Later, when the hardware on IRQ 7 fires, the sequence is:
1. The assembly label `irq7` runs and sets up the stack.
2. `irq_common_stub` calls your C-level `irq_handler`.
3. That code looks up the handler function in an array (like `irq_routines[7]`).
4. Your `myDeviceIrqHandler` runs.

If you see messages or logs from `myDeviceIrqHandler` at the right times, you know the IDT setup worked for IRQ 7.

---

## Example 3: Manually Causing a CPU Fault

Another way to confirm your exception stubs is to deliberately do something that causes an exception. For instance, you could create a **Divide-by-Zero** fault in a small test routine:

```c
void testDivideByZero(void) {
    volatile int x = 0;
    volatile int y = 10 / x; // This is obviously invalid
    (void)y; // just to silence compiler warnings
}
```

When you call `testDivideByZero()`, the CPU triggers the **Divide-by-Zero** exception on interrupt vector 0. In `idt.asm`, you should have `ISR_NOERRCODE 0` for that, which eventually calls your code that handles exceptions. If your screen shows some message or the system halts, that means the IDT entry for vector 0 is working properly.



## **keyboard.c**

```c
#include "keyboard.h"
#include "util.h"     
#include "idt.h"
#include "stdio.h"
```
The file begins by including several headers. **`keyboard.h`** provides function and variable declarations for the keyboard driver. **`util.h`** includes low-level utility functions such as `inPortB`. **`idt.h`** lets this file register its interrupt handler for IRQ 1 (the keyboard line). **`stdio.h`** is used for printing characters or strings to the screen.

```c
const uint32_t CAPS = 0xFFFFFFFF - 29;
bool capsOn = false;
bool capsLock = false;

const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC     = 0xFFFFFFFF - 1;
const uint32_t CTRL    = 0xFFFFFFFF - 2;
const uint32_t LSHFT   = 0xFFFFFFFF - 3;
const uint32_t RSHFT   = 0xFFFFFFFF - 4;
const uint32_t ALT     = 0xFFFFFFFF - 5;
```
A few global constants and booleans are defined here. **`CAPS`** is treated as a special value in the lookup tables, while **`capsOn`** and **`capsLock`** track the current Shift and Caps Lock states. Several **`UNKNOWN`** or **ESC / CTRL / ALT** values are assigned as sentinel markers that help distinguish between recognized ASCII characters and special keys.

```c
const uint32_t lowercase[128] = { ... };
const uint32_t uppercase[128] = { ... };
```
Two arrays of 128 elements each map scan codes (from the keyboard hardware) to ASCII codes. The **lowercase** array is used when neither Shift nor Caps Lock is active; the **uppercase** array is used otherwise. Both are indexed by the keyboard’s scan code, which comes directly from port `0x60`. Some entries are special values like `UNKNOWN` or `ESC`.

```c
void keyboardHandler(struct InterruptRegisters *regs) {
    uint8_t scancode = inPortB(0x60);
    uint8_t keyCode = scancode & 0x7F;
    bool keyPressed = !(scancode & 0x80);
    
    switch (keyCode) {
        ...
        case 42:  // Left Shift
            capsOn = keyPressed;
            break;
        case 58:  // Caps Lock
            if (keyPressed) {
                capsLock = !capsLock;
            }
            break;
        default:
            if (keyPressed) {
                uint32_t ch = (capsOn || capsLock)
                              ? uppercase[keyCode]
                              : lowercase[keyCode];
                if (ch != UNKNOWN) {
                    char s[2] = {(char)ch, '\0'};
                    print(s);
                }
            }
            break;
    }
}
```
The **keyboardHandler** function is called whenever **IRQ 1** fires. It reads one scan code from port `0x60`. The top bit (bit 7) of the scan code tells us whether a key was released (1) or pressed (0). So **`keyPressed`** becomes true only if this bit is zero.

In the `switch` statement, certain scan codes (like **ESC**, **CTRL**, or **ALT**) are ignored here; the code does nothing special with them. For **Left Shift**, the **capsOn** boolean is set or cleared depending on whether the key is pressed or released. For **Caps Lock**, the code toggles **capsLock** only on press events (so pressing once will toggle it on, pressing again toggles it off).

Every other key code is checked to see if it’s a press event. If it is, the code uses either the **lowercase** or **uppercase** array to find an ASCII character. If the result is not **UNKNOWN**, it prints that character to the screen. By combining the **Shift** (capsOn) logic with **Caps Lock** (capsLock), the program can decide which table to look up when the user types.

```c
void initKeyboard() {
    capsOn = false;
    capsLock = false;
    irq_install_handler(1, &keyboardHandler);
}
```
**`initKeyboard()`** resets the Shift and Caps Lock flags and then registers the **keyboardHandler** function with **IRQ 1**. Once this is done, pressing any key on the keyboard will cause the CPU to jump into **keyboardHandler** whenever the hardware signals **IRQ 1**.


## Changes to VGA Driver

### Original Issue: Hardware Cursor Remains Stuck

Previously, the driver maintained only a software cursor using `cursor_x` and `cursor_y`. These values tracked where new characters should be placed in the text buffer at `0xB8000`, but nothing in the code wrote to the VGA hardware registers that control the physical, blinking cursor. As a result, the underscore that appears by default in text mode remained at the initial position (the top-left corner), regardless of where text was actually printed.

### Enabling and Synchronizing the Hardware Cursor

Two new functions were introduced near the top of **vga.c**:  
```c
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) { ... }
void update_cursor(uint16_t x, uint16_t y) { ... }
```
These use ports `0x3D4` and `0x3D5` to control cursor shape and position:

1. **`enable_cursor(cursor_start, cursor_end)`**: Makes the hardware cursor visible, setting up how it looks on screen (for example, an underline). The VGA expects two scanline values for the start and end of the cursor shape. Calling `enable_cursor(14, 15)` results in a blinking underline.

2. **`update_cursor(x, y)`**: Repositions the blinking cursor to the coordinates `(x, y)`. To do that, it calculates a linear index `pos = y * width + x` and writes it to the VGA index registers so the hardware knows where to blink.

In `Reset()`, both functions are called:
```c
cursor_x = cursor_y = 0;
enable_cursor(14, 15);
update_cursor(cursor_x, cursor_y);
```
Right after clearing the screen, the code turns on the cursor as an underline and ensures it starts at (0, 0). Elsewhere in the code, such as in `print(const char* text)` and in `newLine()`, whenever a character is placed or the cursor moves, `update_cursor(cursor_x, cursor_y)` is called again to keep the hardware cursor aligned with the software cursor.

### Correcting Backspace

Another change addressed backspace. The old driver printed a stray character when backspace was pressed, because it was mapped to a printable symbol in the scancode tables and handled the same as normal characters. Now, `'\b'` is handled by moving `cursor_x` back (and possibly going up one line if already at the left edge), then overwriting the old position with a space:

```c
else if (c == '\b') {
    if (cursor_x > 0) {
        cursor_x--;
        VGA_MEMORY[cursor_y * width + cursor_x] = makeVgaCell(' ', currentAttribute);
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = width - 1;
        VGA_MEMORY[cursor_y * width + cursor_x] = makeVgaCell(' ', currentAttribute);
    }
    update_cursor(cursor_x, cursor_y);
}
```
This ensures that pressing backspace removes the previous character on screen rather than inserting a symbol.

### Summary of Improvements

- The blinking underscore is now enabled and accurately follows text entry.  
- The cursor coordinates stored in `cursor_x` and `cursor_y` are mirrored in hardware via `update_cursor(...)`.  
- Backspace erases the most recently printed character instead of printing an unwanted symbol.  

