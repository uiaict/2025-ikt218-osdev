extern "C" {
    #include "terminal.h"    //getting the neeeded header files for the kernel to use 
    #include "gdt.h"
    #include "idt.h"
    #include "irq.h"
    #include "memory.h"
    #include "pit.h"
    #include "song/song.h"
    #include "song/song_player.h"
    #include "song/song_data.h"
    #include "port_io.h"
    #include "piano.h"     
}

extern "C" uint32_t end;

extern "C" void kernel_main() {
    gdt_install();
    irq_remap();  // Remap IRQs before loading IDT
    idt_install();
    init_kernel_memory(&end);
    
    terminal_initialize();  //  Now VGA memory is safely mapped
    terminal_write("Hello from kernel_main!\n");
    init_paging();  // Enable paging BEFORE touching VGA memory

    terminal_write("GDT is installed!\n");
    terminal_write("IRQs remapped!\n");
    terminal_write("IDT is installed!\n");
    terminal_write("Kernel memory manager initialized!\n");
    terminal_write("Paging initialized!\n");

    print_memory_layout();

    init_pit();
    terminal_write("PIT initialized!\n");

    void* a = malloc(1234);
    void* b = malloc(5678);
    terminal_write("Allocated memory!\n");


    __asm__ __volatile__("sti"); //testing


    // Trigger software interrupts (ISRs) can be commented out to stop the triggers
    __asm__ __volatile__("int $0x0");
    __asm__ __volatile__("int $0x3");
    __asm__ __volatile__("int $0x1");

    // Trigger IRQ0 (Timer) for test can also be commented out for testing.
    __asm__ __volatile__("int $0x20");

    terminal_write("Back from interrupts.\n");
    terminal_write("\nPress a key:\n");
    terminal_write("Press keys now:\n");

    terminal_write("Playing epic melody...\n"); //playing the melody
    Song song = {
        .notes = zelda_overworld_theme,
        .note_count = zelda_overworld_theme_length
    };
    play_song_impl(&song);
    
    terminal_write("Finished playing melody.\n"); //done with the melody

    run_piano(); // starts up the piano loop
    

    int counter = 0;
    while (1) {
        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Sleeping with busy-waiting (HIGH CPU)...\n");
        sleep_busy(1000);

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Slept using busy-waiting.\n");
        counter++;

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Sleeping with interrupts (LOW CPU)...\n");
        sleep_interrupt(1000);

        terminal_write("[");
        terminal_putint(counter);
        terminal_write("]: Slept using interrupts.\n");
        counter++;
    }
}
