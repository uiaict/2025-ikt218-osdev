#include "libc/system.h"
#include "libc/stdarg.h"

// Static buffer used for printing trace info
static char buffer[4096];

// Checks if a return address exists at stack frame N
#define frp(N, ra) \
  (__builtin_frame_address(N) != NULL) && \
  (ra = __builtin_return_address(N)) != NULL && ra != (void*)-1

// Prints a single stack frame
static void print_trace(const int N, const void* ra)
{
  printf(buffer, sizeof(buffer),
         "[%d] %p\n",
         N, ra);
  printf(buffer);
}

// Prints up to 3 levels of backtrace
void print_backtrace()
{
  printf("\nBacktrace:\n");
  void* ra;
  if (frp(0, ra)) {
    print_trace(0, ra);
    if (frp(1, ra)) {
      print_trace(1, ra);
      if (frp(2, ra)) {
        print_trace(2, ra);
      }
    }
  }
}

// Called on fatal error, halts the kernel
__attribute__((noreturn))
void panic(const char* reason)
{
  printf("\n\n!!! PANIC !!!\n%s\n", reason);
  print_backtrace();
  printf("\nKernel halting...\n");
  while (1) asm("cli; hlt");
  __builtin_unreachable();
}

// Calls panic with "Abort called"
void abort()
{
  panic("Abort called");
}
