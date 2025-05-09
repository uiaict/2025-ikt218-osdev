[BITS 32]
global load_page_directory
load_page_directory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

global enable_paging
enable_paging:
    ; Enable PSE in CR4
    mov eax, cr4
    or eax, 0x10
    mov cr4, eax

    ; Enable paging in CR0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret
