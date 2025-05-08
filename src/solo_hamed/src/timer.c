#include "timer.h"
#include "isr.h"
#include "monitor.h"

u32int tick = 0;

static void timer_callback(registers_t regs)
{
   (void)regs;
   tick++;
}

void init_timer(u32int frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}

void sleep(u32int ms)
{
   // Calculate the number of ticks we need to wait
   // If timer frequency is 50Hz, each tick is 20ms
   // So we need to wait for ms/20 ticks
   u32int start_tick = tick;
   u32int ticks_to_wait = ms / 20; // Assuming 50Hz frequency

   // If we need to wait for at least 1 tick
   if (ticks_to_wait == 0) ticks_to_wait = 1;

   // Wait until enough ticks have passed
   while (tick < start_tick + ticks_to_wait)
   {
      // Just wait
      asm volatile("hlt");
   }
}
