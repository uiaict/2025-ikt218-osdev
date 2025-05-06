#include "drivers/terminal.h"
#include "libc/math.h"
#include "drivers/keyboard.h"

float ring_radius = 0.25;
float donut_radius = 0.5;
float distance = 8.0;
float scale;

float speed = 5.0;
char message[WIDTH] = "Q: BACK, +/-: CHANGE SPEED";

char buffer[HEIGHT][WIDTH + 1];
float zbuffer[HEIGHT][WIDTH];

void init_buffers() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x] = ' ';
            zbuffer[y][x] = 0;
        }
        buffer[y][WIDTH] = '\0';
    }

    terminal_clear();
}

void clear_buffer() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x] = ' ';
            zbuffer[y][x] = 0;
        }
    }
}

void render_donut(float a, float b) {
    scale = WIDTH * distance * 3 / (8 * (ring_radius + donut_radius)) * 0.4;

    clear_buffer();

    float cos_a = cos(a);
    float sin_a = sin(a);
    float cos_b = cos(b);
    float sin_b = sin(b);
    
    for (float t = 0; t < 2 * PI; t += 0.07) {
        float cos_t = cos(t);
        float sin_t = sin(t);

        for (float p = 0; p < 2 * PI; p += 0.02) {
            float cos_p = cos(p);
            float sin_p = sin(p);

            float circle_x = donut_radius + ring_radius * cos_t;
            float circle_y = ring_radius * sin_t;

            float x = circle_x * (cos_b * cos_p + sin_a * sin_b * sin_p) - circle_y * cos_a * sin_b;
            float y = circle_x * (sin_b * cos_p - sin_a * cos_b * sin_p) + circle_y * cos_a * cos_b;
            float z = distance + cos_a * circle_x * sin_p + circle_y * sin_a;
            float ooz = 1 / z;

            int xp = (int)(WIDTH/2 + scale * ooz * x);
            int yp = (int)(HEIGHT/2 - scale * ooz * y * 0.5);

            float l = cos_p * cos_t *sin_b - cos_a * cos_t *sin_p - sin_a * sin_t + cos_b * (cos_a * sin_t - cos_t * sin_a * sin_p);

            if (l > 0 && xp >= 0 && xp < WIDTH && yp >= 0 && yp < HEIGHT && ooz > zbuffer[yp][xp]) {
                zbuffer[yp][xp] = ooz;
                int luminance_index = (int)(l * 8);
                char lum_char;
                if (luminance_index <= 0) lum_char = '.';
                else if (luminance_index == 1) lum_char = ',';
                else if (luminance_index == 2) lum_char = '-';
                else if (luminance_index == 3) lum_char = '~';
                else if (luminance_index == 4) lum_char = ':';
                else if (luminance_index == 5) lum_char = ';';
                else if (luminance_index == 6) lum_char = '=';
                else if (luminance_index == 7) lum_char = '!';
                else if (luminance_index == 8) lum_char = '*';
                else if (luminance_index == 9) lum_char = '#';
                else if (luminance_index == 10) lum_char = '$';
                else lum_char = '@';
                
                buffer[yp][xp] = lum_char;
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            terminal_put(buffer[y][x], WHITE, x, y);
        }
    }

    for (int i = 0; message[i] != '\0'; i++) {
        terminal_put(message[i], WHITE, i, HEIGHT - 1);
    }

    update_cursor(0, HEIGHT - 1);
}

void animate_donut() {
    float a = 0, b = 0;

    init_buffers();
    disable_cursor();
    
    while (1) {
        render_donut(a, b);
        
        a += speed * 0.04;
        b += speed * 0.02;

        if (is_key_pressed('+')) {
            speed = speed + 1;
        }

        if (is_key_pressed('-')) {
            speed = speed - 1;
        }

        if (is_key_pressed('q')) {
            terminal_clear();
            enable_cursor(0, 0);
            break;
        }
    }
}