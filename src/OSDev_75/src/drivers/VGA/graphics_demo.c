#include "vga_graphics.h"
#include "pit.h"
#include "util.h"

// Define some colors
#define COLOR_BLACK     0
#define COLOR_BLUE      1
#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_MAGENTA   5
#define COLOR_BROWN     6
#define COLOR_LGRAY     7
#define COLOR_DGRAY     8
#define COLOR_LBLUE     9
#define COLOR_LGREEN    10
#define COLOR_LCYAN     11
#define COLOR_LRED      12
#define COLOR_LMAGENTA  13
#define COLOR_YELLOW    14
#define COLOR_WHITE     15

// Ball structure for bouncing ball demo
typedef struct {
    int x, y;          // Position
    int dx, dy;        // Velocity
    int radius;        // Size
    uint8_t color;     // Color
} Ball;

// Initialize a ball with random properties
void init_ball(Ball* ball) {
    // Position between 20 and 300 x, 20 and 180 y
    ball->x = 20 + (get_current_tick() % 280);
    ball->y = 20 + (get_current_tick() % 160);
    
    // Velocity between -3 and 3
    ball->dx = ((get_current_tick() % 7) - 3);
    if (ball->dx == 0) ball->dx = 1;  // Avoid zero velocity
    
    ball->dy = ((get_current_tick() % 7) - 3);
    if (ball->dy == 0) ball->dy = 1;  // Avoid zero velocity
    
    // Radius between 5 and 15
    ball->radius = 5 + (get_current_tick() % 11);
    
    // Color between 1 and 15 (avoid black)
    ball->color = 1 + (get_current_tick() % 15);
}

// Update ball position and handle bouncing
void update_ball(Ball* ball) {
    // Move the ball
    ball->x += ball->dx;
    ball->y += ball->dy;
    
    // Bounce off edges
    if (ball->x - ball->radius <= 0) {
        ball->x = ball->radius;
        ball->dx = -ball->dx;
    }
    if (ball->x + ball->radius >= GRAPHICS_WIDTH) {
        ball->x = GRAPHICS_WIDTH - ball->radius;
        ball->dx = -ball->dx;
    }
    if (ball->y - ball->radius <= 0) {
        ball->y = ball->radius;
        ball->dy = -ball->dy;
    }
    if (ball->y + ball->radius >= GRAPHICS_HEIGHT) {
        ball->y = GRAPHICS_HEIGHT - ball->radius;
        ball->dy = -ball->dy;
    }
}

// Draw a demo screen showing different graphics functions
void draw_demo_screen() {
    // Clear screen with blue
    clear_screen_graphics(COLOR_BLUE);
    
    // Draw header text
    draw_string_graphics(85, 10, "OSDev_75 Graphics Demo", COLOR_WHITE);
    
    // Draw a rectangle outline
    draw_rect(20, 30, 120, 80, COLOR_YELLOW);
    
    // Draw a filled rectangle
    fill_rect(160, 30, 120, 80, COLOR_RED);
    
    // Draw circles
    draw_circle(80, 70, 30, COLOR_GREEN);
    fill_circle(220, 70, 30, COLOR_MAGENTA);
    
    // Draw some lines
    for (int i = 0; i < 10; i++) {
        draw_line(10, 120 + i * 5, 310, 120 + i * 5, COLOR_CYAN);
    }
    
    // Draw "Press Esc to return to text mode" message
    draw_string_graphics(70, 185, "Press Esc to return to text mode", COLOR_WHITE);
}

// Draw bouncing balls demo
void bouncing_balls_demo() {
    // Create some balls
    Ball balls[10];
    for (int i = 0; i < 10; i++) {
        init_ball(&balls[i]);
    }
    
    // Main animation loop
    uint8_t running = 1;
    
    while (running) {
        // Check for exit key (ESC)
        if ((inPortB(0x60) & 0x7F) == 0x01 && !(inPortB(0x60) & 0x80)) {
            running = 0;
            break;
        }
        
        // Clear screen to black
        clear_screen_graphics(COLOR_BLACK);
        
        // Update and draw each ball
        for (int i = 0; i < 10; i++) {
            // Update ball position
            update_ball(&balls[i]);
            
            // Draw ball
            fill_circle(balls[i].x, balls[i].y, balls[i].radius, balls[i].color);
        }
        
        // Draw title
        draw_string_graphics(90, 10, "Bouncing Balls Demo", COLOR_WHITE);
        
        // Draw exit message
        draw_string_graphics(70, 185, "Press Esc to return to text mode", COLOR_WHITE);
        
        // Small delay for animation
        sleep_interrupt(50);
    }
}

// Run graphics demo
void graphics_demo() {
    // Switch to graphics mode
    set_mode_13h();
    
    // Show static demo screen
    draw_demo_screen();
    
    // Wait a moment to show the static screen
    sleep_interrupt(3000);
    
    // Show bouncing balls demo until ESC is pressed
    bouncing_balls_demo();
    
    // Switch back to text mode
    set_mode_text();
}