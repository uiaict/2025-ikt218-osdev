#include "terminal/print.h"
#include "terminal/clear.h"

void start_screen_reveal() {
    clearTerminal();
    printf("\n\n             -----------------------------------------------------\n");
    printf("                     _______ __               __        __        \n");
    printf("                    |     __|  |--.--_--.--.--|  |   __| |        \n");
    printf("                    |__     |     | |_| |  |  |  |__| -  |        \n");
    printf("                    |_______|__|__|_____|_____|_____|____|        \n");
    printf("       __   __                       _______ __                         \n");
    printf("      |  |_|  |.---.-.--.--.-----.  |     __|  |--.-----.-----.-----.-----.\n");
    printf("      |   _   ||  _  |  |  |  -__|  |    |  |     |  _  |  _ _|  -__|     |\n");
    printf("      |__| |__||___._|_____|_____|  |_________|___|_____|___  |_____|__|__|\n");
    printf("                                                        |_____|\n\n");
    printf("             -----------------------------------------------------\n\n");
    printf("                          Should Have Chosen Windows\n\n");
    printf("                           A Tiny OS by Adam Hazel\n\n");
    printf("                          Press any key to continue...\n");
}