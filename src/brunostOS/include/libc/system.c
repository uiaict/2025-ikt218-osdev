#include "libc/system.h"
#include "libc/stddef.h"
#include "libc/stdio.h"

__attribute__((noreturn)) void panic(const char *reason){
    printf("KERNEL PANIC: %s\n\r", reason);
}
void* _impure_ptr = NULL;


void __stack_chk_fail_local()
{
    panic("Stack protector: Canary modified\n\r");
    __builtin_unreachable();
}
__attribute__((used))
void __stack_chk_fail()
{
    panic("Stack protector: Canary modified\n\r");
    __builtin_unreachable();
}

void _exit(int status)
{
    char buffer[64] = {};
    printf("Exit called with status %d\n\r", status);
    buffer[24] = (int)('0') + status;
    panic(buffer);
    __builtin_unreachable();
}