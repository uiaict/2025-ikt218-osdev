#include "uiElements.h"
void display_colors(){
    monitor_write_color(1, " 1: ");
    monitor_write_color(1, "BLUE        ");
    monitor_write_color(9, " 9: ");
    monitor_write_color(9, "LIGHT BLUE\n");
    
    monitor_write_color(2, " 2: ");
    monitor_write_color(2, "GREEN       ");
    monitor_write_color(10, "10: ");
    monitor_write_color(10, "LIGHT GREEN\n");
    
    monitor_write_color(3, " 3: ");
    monitor_write_color(3, "CYAN        ");
    monitor_write_color(11, "11: ");
    monitor_write_color(11, "LIGHT CYAN\n");
    
    monitor_write_color(4, " 4: ");
    monitor_write_color(4, "RED         ");    
    monitor_write_color(12, "12: ");
    monitor_write_color(12, "LIGHT RED\n");
    
    monitor_write_color(5, " 5: ");
    monitor_write_color(5, "MAGENTA     ");
    monitor_write_color(13, "13: ");
    monitor_write_color(13, "LIGHT MAGENTA\n");
    
    monitor_write_color(6, " 6: ");
    monitor_write_color(6, "BROWN       ");
    monitor_write_color(14, "14: ");
    monitor_write_color(14, "LIGHT BROWN\n");
    
    monitor_write_color(7, " 7: ");
    monitor_write_color(7, "LIGHT GREY  ");
    monitor_write_color(8, " 8: ");
    monitor_write_color(8, "DARK GREY\n");

    monitor_write_color(15, "15: ");
    monitor_write_color(15, "WHITE\n");
    
    monitor_write_color(7, "\nPick a color by number (1-15): ");
}

void print_osdev_banner(){
     // Cool blue-cyan themed colors
     monitor_write_color(5, "  ____..--'    ,-----.    .-------.  ____     __  \n");
     monitor_write_color(5, " |        |  .'  .-,  '.  \\  ");monitor_write_color(14,"_(`)_");monitor_write_color(13," \\ \\   \\   /  / \n");
     monitor_write_color(13, " |   .-'  ' / ,-.|  \\ _ \\ | ");monitor_write_color(14,"(_ o._)");monitor_write_color(13,"|  \\  ");monitor_write_color(14,"_.");monitor_write_color(13," /  '  \n");
     monitor_write_color(13, " |.-'.'   /;  \\  '_ /  | :|  ");monitor_write_color(14,"(_,_)");monitor_write_color(13," /   ");monitor_write_color(14,"_( )_");monitor_write_color(13," .'   \n");
     monitor_write_color(10, "    /   _/ |  _`,/ \\ _/  ||   '-.-'___");monitor_write_color(14,"(_ o _)");monitor_write_color(10,"'    \n");
     monitor_write_color(9, "  .'."); monitor_write_color(14,"_( )_ "); monitor_write_color(9,": (  '\\_/ \\   ;|   |   |   |");monitor_write_color(14,"(_,_)");monitor_write_color(9,"'     \n");
     monitor_write_color(9, ".'  "); monitor_write_color(14,"(_'o._)"); monitor_write_color(9," \\ `\"/  \\  ) / |   |   |   `-'  /      \n");
     monitor_write_color(1, "|    ");monitor_write_color(14,"(_,_)");monitor_write_color(1,"|  '. \\_/``\".'  /   )    \\      /       \n");
     monitor_write_color(1, "|_________|    '-----'    `---'     `-..-'        \n");
     // Title and instructions with bright colors
     monitor_write_color(11, "              Welcome to OSDEV 18 Kernel             \n\n");
     
     monitor_write_color(15, "Type 'song' to play music, 'piano' to open piano, 'game' to play text adventure game.\n");
     monitor_write_color(7, "Type 'help' for available commands.\n");
}

void print_commands(){
    monitor_write("Available commands:\n");
        monitor_write("  song - Play a song from the list\n");
        monitor_write("  piano - Open the piano interface\n");
        monitor_write("  game - Play the text adventure game\n");
        monitor_write("  color - change terminal input color\n");
        monitor_write("  q    - Quit the shell\n");
        monitor_write("  help - Display this help message\n");
        monitor_write("  cls  - Clear the screen\n");
}