![UiAOS Kernel](https://img.shields.io/badge/UiAOS-Kernel-blueviolet) ![Spring 2025](https://img.shields.io/badge/Semester-Spring%202025-green) ![Group 33](https://img.shields.io/badge/Group-33-orange)
# Welcome to: The Byte of 33 - Operating System!
# UiAOS Kernel Project (Assignments 1–7)


## 💾 Assignments

### 1. Booting & Bootloaders
- POST, BIOS vs. UEFI  
- Implemented **first-stage** (512 B) bootloader & **second-stage** loader  
- Comparison: Windows Bootmgr, Mac BootX, GRUB, etc.

### 2. VGA Text Output & GDT
- Built a 3-entry **GDT** (null, code, data)  
- Direct writes to VGA buffer (`0xB8000`) for “Hello, World!”  
- Transition to protected mode via `lgdt` + assembly stub

### 3. Interrupts & Keyboard Logger
- Defined full **IDT** & remapped **PIC** (IRQ 0–15 → vectors 32–47)  
- Handlers for exceptions & IRQs, assembly stubs for `iret`  
- **Keyboard ISR**: scancode → ASCII lookup → on-screen echo

### 4. Memory Management & PIT Timer
- Basic **paging** setup (4 GiB flat mapping)  
- **Heap allocator** with free-list (`malloc`/`free`, C++ `new`/`delete`)  
- Configured **PIT** @ 1 kHz, busy-wait & `hlt`-based sleep

### 5. Music Player
- PC-speaker control (PIT channel 2, port 0x61)  
- `play_song()` API in C++ with C linkage  
- Mode switching & **song-skip** on keypress (`n`)

### 6. Enhanced Music Player & Modes
- **Loading Screen**: ASCII art + progress bar
- **Matrix Mode**: digital-rain animation with color cycling & “Rave Mode”  
- **Piano Mode**: map PS/2 keys (A–K) to notes (C–C″) with real-time play  
- **Music Player**: instant interrupt on `n`, `q`, `s`, `b` via PIT-driven loop  
- Robust PS/2 **controller init** & `last_char` tracking

### 7. Final Submission & PR
- Polished all code, docs, and tests  
- Prepared this README, final report, and GitHub pull request

---


## 🎯 Features by Assignment

| # | Topic                                       | Key Deliverables                                                    |
|:-:|---------------------------------------------|---------------------------------------------------------------------|
| 1 | **Boot & Bootloaders**                      | Theory                                                             |
| 2 | **VGA & GDT**                               | Direct VGA writes (`0xB8000`)<br>GDT setup & `lgdt` transition      |
| 3 | **Interrupts & Keyboard Logger**            | IDT & PIC remapping<br>PS/2 ISR → on-screen scancode echo           |
| 4 | **Paging & PIT Timer**                      | 4 GiB flat paging<br>Simple heap allocator<br>PIT-driven sleep      |
| 5 | **Music Player**                            | PC-speaker control via PIT<br>`play_song()` API<br>Skip on `n`      |
| 6 | **Enhanced Music Player & Modes**           | Boot loading animation<br>Matrix “digital rain”<br>Piano Mode<br>Enhanced Music Player |
| 7 | **Finalization & PR**                       | Polished code & docs<br>Comprehensive README & report               |

---


## 👩‍💻 Authors
Group 33 – IKT218-G 25V (Spring 2025)

· Krzysztof Manczak

· Aleksander Einem Wågen

· Teodor Log Bjørvik
