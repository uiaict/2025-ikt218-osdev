#include "game.h"
#define MAX_INPUT 128

Room rooms[] = {
    {
        "Front Counter",
        "You're at the front counter of the weed dispensary. A budtender greets you with a nod.",
        -1, 1, -1, -1,
        false, false
    },
    {
        "Showroom",
        "Shelves of premium strains line the walls. A faint smell of pine and citrus fills the air.",
        0, 2, 3, -1,
        false, false
    },
    {
        "Storage Room",
        "This room is filled with locked cabinets and smell-proof containers. You see a shiny key on a crate.",
        1, -1, -1, -1,
        true, false
    },
    {
        "VIP Lounge",
        "Dimly lit with lava lamps and bean bags. A torch lies beside a hookah.",
        -1, -1, -1, 1,
        false, true
    }
};


static bool game_running = true;

void init_game() {
    game_running = true;
      
    monitor_write("                                                                     \n");
    monitor_write(" (  (                 (                          )                  \n");
    monitor_write(" )\\))(   '   (    (   )\\ )   )      (         ( /(   (   (      (   \n");
    monitor_write("((_)()\\ )   ))\\  ))\\ (()/(  /((    ))\\  (     )\\()) ))\\  )(    ))\\  \n");
    monitor_write("_(())\\_)() /((_)/((_) ((_))(_))\\  /((_) )\\ ) (_))/ /((_)(()\\  /((_) \n");
    monitor_write("\\ \\((_)/ /(_)) (_))   _| | _)((_)(_))  _(_/( | |_ (_))(  ((_)(_))   \n");
    monitor_write(" \\ \\/\\/ / / -_)/ -_)/ _` | \\ V / / -_)| ' \\))|  _|| || || '_|/ -_)  \n");
    monitor_write("  \\_/\\_/  \\___|\\___|\\__,_|  \\_/  \\___||_||_|  \\__| \\_,_||_|  \\___|  \n");
    monitor_write("                                                                     \n");    
    monitor_write("               WELCOME TO DISPENSARY DUNGEON!               \n");
    monitor_write("   Explore the halls, collect your gear, and light the path!\n\n");
    monitor_write("Type 'help' for available commands.\n");    
}

void run_game() {
    char input[MAX_INPUT];

    GameState state = {
        .current_room = 0,
        .has_key = false,
        .has_torch = false
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
        monitor_write(current->description);
        monitor_write("\n");

    } else if (strcmp(command, "go north") == 0) {
        if (current->north != -1) {
            state->current_room = current->north;
            monitor_write("You go north.\n");
            monitor_write(rooms[state->current_room].description);
            monitor_write("\n");
        } else {
            monitor_write("You can't go north.\n");
        }
    
    } else if (strcmp(command, "go south") == 0) {
        if (current->south != -1) {
            state->current_room = current->south;
            monitor_write("You go south.\n");
            monitor_write(rooms[state->current_room].description);
            monitor_write("\n");
        } else {
            monitor_write("You can't go south.\n");
        }
    
    } else if (strcmp(command, "go east") == 0) {
        if (current->east != -1) {
            state->current_room = current->east;
            monitor_write("You go east.\n");
            monitor_write(rooms[state->current_room].description);
            monitor_write("\n");
        } else {
            monitor_write("You can't go east.\n");
        }
    
    } else if (strcmp(command, "go west") == 0) {
        if (current->west != -1) {
            state->current_room = current->west;
            monitor_write("You go west.\n");
            monitor_write(rooms[state->current_room].description);
            monitor_write("\n");
        } else {
            monitor_write("You can't go west.\n");
        }
      

    } else if (strcmp(command, "take key") == 0 && current->has_key) {
        state->has_key = 1; //updates player's state
        current->has_key = 0; //updates room state
        monitor_write("You took the key.\n");
    } else if (strcmp(command, "take torch") == 0 && current->has_torch) {
        state->has_torch = 1;
        current->has_torch = 0;
        monitor_write("You took the torch.\n");

    } else if (strcmp(command, "inventory") == 0) {
        bool empty = true;
        if (state->has_key) {
            monitor_write("You have a rusty key.\n");
            empty = false;
        }
        if (state->has_torch) {
            monitor_write("You have a torch.\n");
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