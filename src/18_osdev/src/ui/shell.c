#include "shell.h"
// Global variables
static bool shell_running = true;
volatile bool stop_song_requested = false;
volatile int color = 15;

Song all_songs[] = {
    { .name = "mario theme",      .notes = music_1,            .length = sizeof(music_1) / sizeof(Note) },
    { .name = "star wars theme",  .notes = starwars_theme,     .length = sizeof(starwars_theme) / sizeof(Note) },
    { .name = "battlefield 1942", .notes = battlefield_1942_theme, .length = sizeof(battlefield_1942_theme) / sizeof(Note) },
    { .name = "music 2",          .notes = music_2,            .length = sizeof(music_2) / sizeof(Note) },
    { .name = "music 3",          .notes = music_3,            .length = sizeof(music_3) / sizeof(Note) },
    { .name = "music 4",          .notes = music_4,            .length = sizeof(music_4) / sizeof(Note) },
    { .name = "music 5",          .notes = music_5,            .length = sizeof(music_5) / sizeof(Note) },
    { .name = "music 6",          .notes = music_6,            .length = sizeof(music_6) / sizeof(Note) }
};

void init_shell() {
   print_osdev_banner();
}

void run_shell() {
    char input[128];
    shell_running = true;
    
    while (shell_running) {
        // Display prompt and get input
        monitor_write("> ");
        read_line(input);
        
        // Process the command
        process_command(input);
    }
    
    monitor_write("\nShell exited.\n");
}

// Function to check if a key has been pressed while playing
// This should be called by the song player periodically
bool should_stop_song() {
    return stop_song_requested;
}

// Reset the stop flag (call before starting a new song)
void reset_stop_flag() {
    stop_song_requested = false;
}

void process_command(char* command) {
    if (strcmp(command, "song") == 0) {
        monitor_write("Choose song :\n1. Mario\n2. Star Wars\n3. Battlefield 1942\n4. Music 2\n5. Music 3\n6. Music 4\n7. Music 5\n8. Music 6\n> ");
        
        char selection[128];
        read_line(selection);
        int song_id = atoi(selection);

        if (song_id < 1 || song_id > 9) {
            monitor_write("Invalid selection.\n");
            return;
        }

        disable_speaker();

        reset_stop_flag();
        play_song_impl(&all_songs[song_id-1]);
       
        disable_speaker();
    } 

    else if (strcmp(command, "piano") == 0) {
        init_piano();
    } 
    else if (strcmp(command, "game") == 0) {
        run_game();
    } 
    else if(strcmp(command, "color") == 0){
        // Display all color options with their numbers
        display_colors();
        
        char selection[128];
        read_line(selection);
        int new_color = atoi(selection);

        if (new_color < 1 || new_color > 15) {
            monitor_write("Invalid selection.\n");
            return;
        }

        color = new_color ;
    }

    else if (strcmp(command, "stop") == 0) {
        stop_song_requested = true;
        monitor_write("Stopping song...\n");
    }
    
    else if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
        monitor_write("Exiting shell...\n");
        shell_running = false;
    } 

    else if (strcmp(command, "help") == 0) {
        print_commands();
    }

    else if (strcmp(command, "cls") == 0 || strcmp(command, "clear") == 0) {
        monitor_clear();
    }

    else if (strlen(command) > 0) {
        monitor_write("Unknown command: ");
        monitor_write(command);
        monitor_write("\n");
    }
}
