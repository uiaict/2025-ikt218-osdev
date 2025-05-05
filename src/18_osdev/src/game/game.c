#include "game.h"
#define MAX_INPUT 128

Room rooms[] = {
    {
        "Front Counter",
        "You're at the front counter of the weed dispensary. A budtender greets you with a nod.",
        -1, 1, -1, -1,
        false, false, false, false, false
    },
    {
        "Showroom",
        "Shelves of premium strains line the walls. A torch lies beside a hookah.",
        0, 2, 3, -1,
        false, true, false, false, false
    },
    {
        "Storage Room",
        "This room is filled with locked cabinets and smell-proof containers. You see a shiny key on a crate.",
        1, 7, -1, -1,
        true, false, false, false, false
    },
    {
        "VIP Lounge",
        "Dimly lit with lava lamps and bean bags. A faint smell of pine and citrus fills the air.",
        -1, -1, 5, 1,
        false, false, false, true, false
    },
    {
        "Secret Grow Room",
        "The glow of UV lights reveals rows of vibrant plants. It's humid and buzzing with fans.",
        -1, 6, -1, 8,
        false, false, false, true, false
    },
    {
        "Hydro Lab",
        "Pipes, pumps, and nutrients galore. You can hear bubbling water and the faint hum of machines. In the corner you see a key",
        -1, -1, -1, 3,
        true, false, true, false, false
    },
    {
        "Security Hallway",
        "Flashing red lights and motion sensors line the hallway. It's pitch black without a torch, but you see the shine of a key.",
        4, -1, 7, -1,
        true, false, true, false, false
    },
    {
        "Owner's Office",
        "A sleek office with glass walls, a safe, and luxury rolling gear. You feel like you're being watched.",
        2, -1, -1, 6,
        false, true, false, false, false
    },
    {
        "Back Alley Escape",
        "A quiet back door with a view of the city lights. You've found the exit!",
        -1, -1, -1, -1,
        false, false, false, true, true
    }
};



static bool game_running = true;

void init_game() {
    game_running = true;
    
    // Flame colors (red/orange gradients)
    monitor_write_color(12, "                                                                     \n");
    monitor_write_color(12, " (  (                 (                          )                  \n");
    monitor_write_color(12, " )\\))(   '   (    (   )\\ )   )      (         ( /(   (   (      (   \n");
    monitor_write_color(4, "((_)()\\ )   ))\\  ))\\ (()/(  /((    ))\\  (     )\\()) ))\\  )(    ))\\  \n");
    monitor_write_color(4, "_(())\\_)() /((_)/((_) ((_))(_))\\  /((_) )\\ ) (_))/ /((_)(()\\  /((_) \n");
    monitor_write_color(6, "\\ \\((_)/ /(_)) (_))   _| | _)((_)(_))  _(_/( | |_ (_))(  ((_)(_))   \n");
    monitor_write_color(6, " \\ \\/\\/ / / -_)/ -_)/ _` | \\ V / / -_)| ' \\))|  _|| || || '_|/ -_)  \n");
    monitor_write_color(5, "  \\_/\\_/  \\___|\\___|\\__,_|  \\_/  \\___||_||_|  \\__| \\_,_||_|  \\___|  \n");
    monitor_write_color(5, "                                                                     \n");
    
    // Title in bright color
    monitor_write_color(14, "               WELCOME TO DISPENSARY DUNGEON!               \n");
    monitor_write_color(7, "   Explore the halls, collect your gear, and light the path!\n\n");
    monitor_write_color(15, "Type 'help' for available commands.\n");
}

void run_game() {
    char input[MAX_INPUT];

    GameState state = {
        .current_room = 0,
        .has_key = 0,
        .has_torch = 0
    };
    
    init_game();
    while (game_running) {

        monitor_write("> ");
        read_line(input);
        
        process_game_command(input, &state);
    }
    
    monitor_write("\nShell exited.\n");
}

void process_game_command(char* command, GameState* state) {
    Room* current = &rooms[state->current_room];

    if (strcmp(command, "look") == 0) {
        monitor_write(current->name);
        monitor_write("\n");
        monitor_write(current->description);
        monitor_write("\n");

    } else if (strcmp(command, "go north") == 0) {
        try_move(state, rooms, "north");
    
    } else if (strcmp(command, "go south") == 0) {
        try_move(state, rooms, "south");
    
    } else if (strcmp(command, "go east") == 0) {
        try_move(state, rooms, "east");
    
    } else if (strcmp(command, "go west") == 0) {
        try_move(state, rooms, "west");
      

    } else if (strcmp(command, "take key") == 0 && current->has_key) {
        state->has_key = state->has_key + 1; //updates player's state
        current->has_key = 0; //updates room state
        monitor_write("You took the key.\n");
    } else if (strcmp(command, "take torch") == 0 && current->has_torch) {
        state->has_torch = state->has_torch + 1;
        current->has_torch = 0;
        monitor_write("You took the torch.\n");

    } else if (strcmp(command, "inventory") == 0) {
        bool empty = true;
        if (state->has_key>0) {
            monitor_write("You have ");
            monitor_write_dec(state->has_key);
            monitor_write(" key\n");
            empty = false;
        }
        if (state->has_torch>0) {
            monitor_write("You have ");
            monitor_write_dec(state->has_torch);
            monitor_write(" torch\n");
            empty = false;
        }
        if (empty) {
            monitor_write("You have nothing.\n");
        }
    
    } else if (strcmp(command, "help") == 0) {
        monitor_write("Available commands:\n");
        monitor_write("  look - Look around the room\n");
        monitor_write("  go east - Go east\n");
        monitor_write("  go west - Go west\n");
        monitor_write("  go north - Go north\n");
        monitor_write("  go south - Go south\n");
        monitor_write("  take key - Take the key\n");
        monitor_write("  take torch - Take the torch\n");
        monitor_write("  inventory - Check your inventory\n");

    } else if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
        game_running = false;

    }else {
        monitor_write("Unknown command, use 'help' to see all commands.\n");
    }
}

void try_move(GameState *state, Room rooms[], const char *direction) {
    // gets the current room
    Room *current = &rooms[state->current_room];
    int next_index = -1;

    // checks which direction the player wants to go
    if (strcmp(direction, "north") == 0) next_index = current->north;
    else if (strcmp(direction, "south") == 0) next_index = current->south;
    else if (strcmp(direction, "east") == 0) next_index = current->east;
    else if (strcmp(direction, "west") == 0) next_index = current->west;

    // checks if the direction the users wants to go is valid
    if (next_index == -1) {
        monitor_write("You can't go that way.\n");
        return;
    }

    // gets the next room
    Room *next = &rooms[next_index];

    // Handle locked doors
    if (next->is_locked) {
        if (state->has_key) {
            monitor_write("The door is locked. Do you want to use your key? (yes/no)\n");
            char answer[10];
            read_line(answer);
            if (strcmp(answer, "yes") == 0) {
                monitor_write("You use the key to unlock the door.\n");
                next->is_locked = false; // unlock the door
                state->has_key = state->has_key - 1; // consume the key
            } else {
                monitor_write("You decide not to use the key.\n");
                return;
            }
        } else {
            monitor_write("The door is locked. You need a key.\n");
            return;
        }
    }

    // Move to the next room
    state->current_room = next_index;

    monitor_write("You go ");
    monitor_write(direction);
    monitor_write(".\n");
    
    


    // Exit check
    if (next->is_exit) {
        monitor_write("You found the exit! Well done.\n");
        monitor_write("$$\\     $$\\  $$$$$$\\  $$\\   $$\\   $$\\      $$\\ $$$$$$\\$$\\   $$\\\n");
        monitor_write("\\$$\\   $$  |$$  __$$\\ $$ |  $$ |    $$ | $\\  $$ |\\_$$  _|$$$\\  $$ |\n");
        monitor_write(" \\$$\\ $$  / $$ /  $$ |$$ |  $$ |     $$ |$$$\\ $$ |  $$ |   $$$$\\ $$ |\n");
        monitor_write("  \\$$$$  /  $$ |  $$ |$$ |  $$ |      $$ $$ $$\\$$ |  $$ |   $$ $$\\$$ |\n");
        monitor_write("   \\$$  /   $$ |  $$ |$$ |  $$ |      $$$$  _$$$$ |   $$ |   $$ \\$$$$ |\n");
        monitor_write("    $$ |    $$ |  $$ |$$ |  $$ |       $$$  / \\$$$ |  $$ |   $$ |\\$$$ |\n");
        monitor_write("    $$ |     $$$$$$  |\\$$$$$$  |      $$  /   \\$$ |$$$$$$\\ $$ | \\$$ |\n");
        monitor_write("    \\__|     \\______/  \\______/       \\__/     \\__|\\______|\\__|  \\__|\n");
        monitor_write("                                                                      \n");
        return;
    }

    // Handle darkness
    if (next->is_dark) {
        if (state->has_torch) {
            monitor_write("It's dark. Use your torch? (yes/no)\n");
            char torch_answer[10];
            read_line(torch_answer);
            if (strcmp(torch_answer, "yes") == 0) {
                monitor_write("You light the torch and look around.\n");
                state->has_torch = state->has_torch - 1; // consume the torch
                monitor_write(next->description);
                monitor_write("\n");
            } else {
                monitor_write("You stay in the dark. It's hard to see.\n");
            }
        } else {
            monitor_write("It's too dark to see anything.\n");
        }
    } else {
        monitor_write(next->description);
        monitor_write("\n");
    }
}
