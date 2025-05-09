#include "paint.h"
#include "io.h"
#include "memory/memutils.h"
#include "libc/stdio.h"
#include "timer.h"

extern char *video_memory;
extern int cursor_xpos;
extern int cursor_ypos;

void paint(char storage1[], char storage2[]){
    clear_terminal();
    reset_cursor_pos();
    print_menu();
    set_vga_color(VGA_LIGHT_GREY, VGA_WHITE);
    cursor_xpos = 0;
    cursor_ypos = 2;
    update_cursor();

    bool basic_mode = true;
    bool freewrite_state = get_freewrite_state();
    if (freewrite_state){
        set_freewrite(false);
    }
    enable_cursor(0, 15);
    int c = 0;

    while (c != 1){
        c = getchar();
        int x = cursor_xpos;
        int y = cursor_ypos;
        enum vga_color txt_clr = get_vga_txt_clr();
        enum vga_color bg_clr = get_vga_bg_clr();

        switch (c){
            case '2': // down
                cursor_ypos++;
                verify_cursor_pos();
                break;
            case '4': // left
                cursor_xpos--;
                verify_cursor_pos();
                break;
            case '6': // right
                cursor_xpos++;
                verify_cursor_pos();
                break;
            case '8': // up
                cursor_ypos--;
                verify_cursor_pos();
                break;
            case '\n':
                printf(" \b");
                break;
            case ' ':
                printf(" \b");
                break;

            case 'R':
                set_vga_color(VGA_WHITE, VGA_RED);
                break;
            case 'G':
                set_vga_color(VGA_WHITE, VGA_GREEN);
                break;
            case 'B':
                set_vga_color(VGA_WHITE, VGA_BLUE);
                break;
            case 'C':
                set_vga_color(VGA_WHITE, VGA_CYAN);
                break;
            case 'M':
                set_vga_color(VGA_WHITE, VGA_MAGENTA);
                break;
            case 'Y':
                set_vga_color(VGA_WHITE, VGA_GREY);
                break;
            case 'K':
                set_vga_color(VGA_WHITE, VGA_BLACK);
                break;
            case 'k':
                set_vga_color(VGA_WHITE, VGA_BLACK);
                break;
            case 'r':
                set_vga_color(VGA_WHITE, VGA_LIGHT_RED);
                break;
            case 'g':
                set_vga_color(VGA_WHITE, VGA_LIGHT_GREEN);
                break;
            case 'b':
                set_vga_color(VGA_WHITE, VGA_LIGHT_BLUE);
                break;
            case 'c':
                set_vga_color(VGA_WHITE, VGA_LIGHT_CYAN);
                break;
            case 'm':
                set_vga_color(VGA_WHITE, VGA_LIGHT_MAGENTA);
                break;
            case 'y':
                set_vga_color(VGA_WHITE, VGA_LIGHT_GREY);
                break;
            case 'W':
                set_vga_color(VGA_GREY, VGA_WHITE);
                break;
            case 'w':
                set_vga_color(VGA_GREY, VGA_WHITE);
                break;
            case 'Q':
                cursor_xpos = 11;
                cursor_ypos = 0;
                set_vga_color(VGA_BLACK, VGA_WHITE);
                if (basic_mode){
                    printf("swift");
                } else{
                    printf("basic");
                }
                set_vga_color(txt_clr, bg_clr);
                basic_mode = !basic_mode;
                cursor_xpos = x;
                cursor_ypos = y;
                break;
            case 'q':
                cursor_xpos = 11;
                cursor_ypos = 0;
                set_vga_color(VGA_BLACK, VGA_WHITE);
                if (basic_mode){
                    printf("swift");
                } else{
                    printf("basic");
                }
                set_vga_color(txt_clr, bg_clr);
                basic_mode = !basic_mode;
                cursor_xpos = x;
                cursor_ypos = y;
                break;
            case 'S':
                save_painting(storage1, storage2);
                break;
            case 's':
                save_painting(storage1, storage2);
                break;
            case 'L':
                load_painting(storage1, storage2);
                break;
            case 'l':
                load_painting(storage1, storage2);
                break;


            default:
                break;
        }
        // update selected color
        x = cursor_xpos;
        y = cursor_ypos;
        cursor_xpos = 9;
        cursor_ypos = 1;
        printf("selected color");
        cursor_xpos = x;
        cursor_ypos = y;

        // keeps cursor within border
        if (cursor_ypos < 2){
            cursor_ypos = 2;
        } else if (cursor_ypos > 22){
            cursor_ypos = 22;
        }
        
        if (!basic_mode){
            // if swift mode, paint on all keypress
            printf(" \b");
        } 
        update_cursor();
    }
    
    // restore before exit
    set_freewrite(freewrite_state);
    enable_cursor(14, 15);
    reset_cursor_pos();
    set_vga_color(VGA_WHITE, VGA_BLACK);
    clear_terminal();
    update_cursor();
}

void print_menu(){
    cursor_xpos = 0;
    cursor_ypos = 0;
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREY);
    for (size_t i = 0; i < VGA_WIDTH*2; i++){
        printf(" ");
    }
    cursor_xpos = 0;
    cursor_ypos = 0;    

    set_vga_color(VGA_WHITE, VGA_RED);
    printf("R");
    set_vga_color(VGA_WHITE, VGA_GREEN);
    printf("G");
    set_vga_color(VGA_WHITE, VGA_BLUE);
    printf("B");
    set_vga_color(VGA_WHITE, VGA_CYAN);
    printf("C");
    set_vga_color(VGA_WHITE, VGA_MAGENTA);
    printf("M");
    set_vga_color(VGA_WHITE, VGA_GREY);
    printf("Y");
    set_vga_color(VGA_WHITE, VGA_BLACK);
    printf("K");

    set_vga_color(VGA_BLACK, VGA_LIGHT_GREY);
    printf("  ");
    set_vga_color(VGA_BLACK, VGA_WHITE);
    printf("Q:basic mode\n\r");

    set_vga_color(VGA_WHITE, VGA_LIGHT_RED);
    printf("r");
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREEN);
    printf("g");
    set_vga_color(VGA_WHITE, VGA_LIGHT_BLUE);
    printf("b");
    set_vga_color(VGA_WHITE, VGA_LIGHT_CYAN);
    printf("c");
    set_vga_color(VGA_WHITE, VGA_LIGHT_MAGENTA);
    printf("m");
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREY);
    printf("y");
    set_vga_color(VGA_BLACK, VGA_WHITE);
    printf("W");

    set_vga_color(VGA_BLACK, VGA_LIGHT_GREY);
    printf("  ");
    set_vga_color(VGA_GREY, VGA_WHITE);
    printf("selected color");

    set_vga_color(VGA_BLACK, VGA_WHITE);
    cursor_xpos = 72;
    cursor_ypos = 0;
    printf("S:save");
    cursor_xpos = 72;
    cursor_ypos = 1;
    printf("L:load");

    cursor_xpos = 0;
    cursor_ypos = 23;
    set_vga_color(VGA_WHITE, VGA_LIGHT_GREY);
    for (size_t i = 0; i < VGA_WIDTH * 2; i++){
        printf(" ");
    }
    cursor_xpos = 1;
    cursor_ypos = 23;
    set_vga_color(VGA_BLACK, VGA_WHITE);
    printf("ESC:exit");
    
    cursor_xpos = 0;
    cursor_ypos = 2;
    set_vga_color(VGA_GREY, VGA_WHITE);
}


void savemenu_clear(int l){
    set_vga_color(VGA_BLACK, VGA_WHITE);
    cursor_xpos = 60;
    cursor_ypos = l;
    for (size_t i = 0; i < 18; i++){
        printf(" ");
    }
    cursor_xpos = 60;
    cursor_ypos = l;
}

void save_painting(char storage1[], char storage2[]){
    savemenu_clear(0);
    savemenu_clear(1);
    disable_cursor();

    struct save_header *h1 = (struct save_header*)storage1;
    struct save_header *h2 = (struct save_header*)storage2;

    if (h1->magic == magic){
        cursor_xpos = 51;
        cursor_ypos = 0;
        printf("storageA:%s", h1->filename);
    } else{
        cursor_xpos = 51;
        cursor_ypos = 0;
        printf("storageA:empty storage");
    }
    
    if (h2->magic == magic){
        cursor_xpos = 51;
        cursor_ypos = 1;
        printf("storageB:%s", h1->filename);
    } else{
        cursor_xpos = 51;
        cursor_ypos = 1;
        printf("storageB:empty storage");
    }
 
    int c = getchar();

    if (c == 'A' || c == 'a'){
        savemenu_clear(0);
        enable_cursor(14,15);
        update_cursor();
        set_freewrite(true);
        memset(h1->filename, 0, sizeof(h1->filename));
        scanf("%s", h1->filename);
        h1->filename[20] = '\0';
        h1->magic = magic;
        savemenu_clear(0);
        h1 += sizeof(struct save_header);
        memcpy(h1, video_memory, PIXEL_COUNT);
        printf("painting saved");
        getchar();  // press any key to continue

    } else if (c == 'B' || c == 'b'){ // down is bound to '2'
        savemenu_clear(1);
        enable_cursor(14,15);
        update_cursor();
        set_freewrite(true);
        scanf("%s", h2->filename);
        h2->filename[20] = '\0';
        h2->magic = magic;
        savemenu_clear(1);
        h2 += sizeof(struct save_header);
        memcpy(h2, video_memory, PIXEL_COUNT);
        printf("painting saved");
        getchar();  // press any key to continue
    }

    enable_cursor(0,15);
    set_freewrite(false);
    print_menu();
}

void load_painting(char storage1[], char storage2[]){
    savemenu_clear(0);
    savemenu_clear(1);
    disable_cursor();

    bool is_empty1 = true;
    bool is_empty2 = true;

    struct save_header *h1 = (struct save_header*)storage1;
    struct save_header *h2 = (struct save_header*)storage2;

    if (h1->magic == magic){
        is_empty1 = false;
        cursor_xpos = 51;
        cursor_ypos = 0;
        printf("storageA:%s", h1->filename);
    } else{
        cursor_xpos = 51;
        cursor_ypos = 0;
        printf("storageA:empty storage");
    }
    
    if (h2->magic == magic){
        is_empty2 = false;
        cursor_xpos = 51;
        cursor_ypos = 1;
        printf("storageB:%s", h2->filename);
    } else{
        cursor_xpos = 51;
        cursor_ypos = 1;
        printf("storageB:empty storage");
    }
 
    int c = getchar();

    if (!is_empty1 && (c == 'A' || c == 'a')){
        h1 += sizeof(struct save_header);
        memcpy(video_memory, h1, PIXEL_COUNT);
        savemenu_clear(0);
        printf("painting loaded");
        getchar();  // press any key to continue

    } else if (!is_empty1 && (c == 'B' || c == 'b')){ // down is bound to '2'
        h2 += sizeof(struct save_header);
        memcpy(video_memory, h2, PIXEL_COUNT);
        savemenu_clear(1);
        printf("painting loaded");
        getchar();  // press any key to continue

    } else if (is_empty1 && (c == 'A' || c == 'a')){
        savemenu_clear(0);
        printf("storageA is empty");
        getchar();

    } else if (is_empty2 && (c == 'B' || c == 'b')){
        savemenu_clear(1);
        printf("storageA is empty");
        getchar();
    }
    

    enable_cursor(0,15);
    print_menu();
}
