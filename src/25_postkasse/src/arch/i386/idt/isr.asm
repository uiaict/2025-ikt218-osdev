[bits 32]

global isr0
global isr1
global isr2

isr0:
    cli
    pushad
    popad
    iretd

isr1:
    cli
    pushad
    popad
    iretd

isr2:
    cli
    pushad
    popad
    iretd
