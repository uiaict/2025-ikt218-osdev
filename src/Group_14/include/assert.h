/**
 * @file assert.h
 * @brief Kernel Assertion and Panic Handlers
 */
 #ifndef ASSERT_H
 #define ASSERT_H
 
 #include "terminal.h" // For terminal_printf
 
 // --- Standard Stringification Macros ---
 // These are necessary to correctly convert __LINE__ (an integer) into a string literal
 // within other macros.
 #define TOSTRING(x) #x
 #define STRINGIFY(x) TOSTRING(x)
 
 // --- Kernel Panic Handler ---
 // Prints a panic message with file/line information and halts the system.
 // Use this for unrecoverable errors.
 #ifndef KERNEL_PANIC_HALT // Guard against potential redefinition
 #define KERNEL_PANIC_HALT(msg) do { \
     asm volatile ("cli"); /* Disable interrupts FIRST */ \
     terminal_printf("\n[KERNEL PANIC] %s\n", msg); \
     terminal_printf("   at %s:%d\n", __FILE__, __LINE__); /* Use %d for __LINE__ */ \
     terminal_printf("System Halted.\n"); \
     while(1) { asm volatile ("hlt"); } /* Halt the CPU */ \
 } while(0)
 #endif // KERNEL_PANIC_HALT
 
 
 /**
  * @brief Kernel Assertion Check
  * If the condition `expr` evaluates to false (0), prints a detailed error message
  * including the failed expression, the provided message, file name, and line number,
  * then triggers a kernel panic.
  *
  * @param expr The expression to evaluate. Should evaluate to true if valid.
  * @param msg A descriptive message string explaining the assertion failure.
  */
 #define KERNEL_ASSERT(expr, msg) do { \
     if (!(expr)) { \
         terminal_printf("\n[ASSERT FAILED] %s\n", msg); \
         terminal_printf("   Expression: %s\n", #expr); \
         /* KERNEL_PANIC_HALT below will print file/line of the ASSERT call site */ \
         KERNEL_PANIC_HALT("Assertion failed"); /* Trigger panic */ \
     } \
 } while (0)
 
 
 // --- Standard assert Macro (Optional) ---
 // Provides a standard assert interface, typically disabled in release builds.
 #ifdef DEBUG
     // In Debug builds, map assert directly to KERNEL_ASSERT
     #define assert(expr) KERNEL_ASSERT(expr, "Assertion failed")
 #else
     // In Release builds, compile assert away to nothing
     #define assert(expr) ((void)0)
 #endif
 
 
 #endif // ASSERT_H