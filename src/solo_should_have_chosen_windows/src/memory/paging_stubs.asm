global load_page_directory
global enable_paging

section .text

; Load page directory (CR3)
; Takes a pointer to the page directory in EAX (passed on stack)
load_page_directory:
    mov eax, [esp + 4]    ; get the page directory pointer (1st arg)
    mov cr3, eax          ; load it into CR3
    ret

; Enable paging by setting bit 31 (PG) of CR0
enable_paging:
    mov eax, cr0
    or eax, 0x80000000    ; set PG bit
    mov cr0, eax
    ret
