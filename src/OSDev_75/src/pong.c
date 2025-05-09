#include "pong.h"
#include "arch/i386/GDT/util.h"
#include "libc/string.h"
#include "drivers/PIT/pit.h"

PongGame pong;

Note bounce_notes[] = {{C5, 50}, {E5, 50}};
Note score_notes[] = {{G4, 100}, {C5, 150}, {G5, 200}};
Song bounce_sound = {bounce_notes, sizeof(bounce_notes) / sizeof(Note)};
Song score_sound = {score_notes, sizeof(score_notes) / sizeof(Note)};

float ball_speed_multiplier = 1.0;
float ai_skill_bonus = 0.0;
float ai_prediction_accuracy = 0.7;

#define UIA_RED        4
#define UIA_WHITE     15

const char* top_border = "╔══════════════════════════════════════════════════════════════════════════╗";
const char* bottom_border = "╚══════════════════════════════════════════════════════════════════════════╝";
const char* side_border = "║";

void init_pong() {
    pong.left_paddle.x = 2;
    pong.left_paddle.y = PONG_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    pong.left_paddle.score = 0;
    
    pong.right_paddle.x = PONG_WIDTH - 3;
    pong.right_paddle.y = PONG_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    pong.right_paddle.score = 0;
    
    ball_speed_multiplier = 1.0;
    ai_skill_bonus = 0.0;
    ai_prediction_accuracy = 0.7;
    reset_ball();
    
    pong.running = true;
    pong.last_update_time = get_current_tick();
    pong.difficulty = DIFFICULTY_MEDIUM;
}

void reset_ball() {
    pong.ball.x = PONG_WIDTH / 2;
    pong.ball.y = PONG_HEIGHT / 2;
    
    if (get_current_tick() % 2 == 0) {
        pong.ball.vel_x = 0.5 * ball_speed_multiplier;
    } else {
        pong.ball.vel_x = -0.5 * ball_speed_multiplier;
    }
    
    pong.ball.vel_y = ((float)(get_current_tick() % 5) - 2.0) / 10.0 * ball_speed_multiplier;
}

void play_bounce_sound() {
    SongPlayer* player = create_song_player();
    player->play_song(&bounce_sound);
}

void play_score_sound() {
    SongPlayer* player = create_song_player();
    player->play_song(&score_sound);
}

float predict_ball_y_position() {
    if (pong.ball.vel_x <= 0) {
        return pong.ball.y;
    }
    
    float steps = (pong.right_paddle.x - pong.ball.x) / pong.ball.vel_x;
    float predicted_y = pong.ball.y + (pong.ball.vel_y * steps);
    
    while (predicted_y < 1 || predicted_y > PONG_HEIGHT - 1) {
        if (predicted_y < 1) {
            predicted_y = 2 - predicted_y;
        } else if (predicted_y > PONG_HEIGHT - 1) {
            predicted_y = 2 * (PONG_HEIGHT - 1) - predicted_y;
        }
    }
    
    float random_offset = ((float)(get_current_tick() % 10) - 5.0);
    float adjusted_accuracy = ai_prediction_accuracy + (ai_skill_bonus * 0.05);
    
    if (adjusted_accuracy < 0.2) adjusted_accuracy = 0.2;
    if (adjusted_accuracy > 0.95) adjusted_accuracy = 0.95;
    
    return predicted_y * adjusted_accuracy + (pong.ball.y + random_offset) * (1.0 - adjusted_accuracy);
}

void update_pong() {
    uint32_t current_time = get_current_tick();
    
    if (current_time - pong.last_update_time < 50) {
        return;
    }
    
    pong.last_update_time = current_time;
    
    pong.ball.x += pong.ball.vel_x;
    pong.ball.y += pong.ball.vel_y;
    
    if (pong.ball.y <= 1 || pong.ball.y >= PONG_HEIGHT - 1) {
        pong.ball.vel_y = -pong.ball.vel_y;
        play_bounce_sound();
    }
    
    if (pong.ball.x <= pong.left_paddle.x + PADDLE_WIDTH &&
        pong.ball.y >= pong.left_paddle.y &&
        pong.ball.y <= pong.left_paddle.y + PADDLE_HEIGHT) {
        
        pong.ball.vel_x = -pong.ball.vel_x;
        
        float rel_intersect = (pong.left_paddle.y + (PADDLE_HEIGHT / 2)) - pong.ball.y;
        float norm_rel_intersect = rel_intersect / (PADDLE_HEIGHT / 2);
        pong.ball.vel_y = -norm_rel_intersect * 0.5;
        
        pong.ball.vel_x *= 1.05;
        ball_speed_multiplier += 0.02;
        
        play_bounce_sound();
    }
    
    if (pong.ball.x >= pong.right_paddle.x - 1 &&
        pong.ball.y >= pong.right_paddle.y &&
        pong.ball.y <= pong.right_paddle.y + PADDLE_HEIGHT) {
        
        pong.ball.vel_x = -pong.ball.vel_x;
        
        float rel_intersect = (pong.right_paddle.y + (PADDLE_HEIGHT / 2)) - pong.ball.y;
        float norm_rel_intersect = rel_intersect / (PADDLE_HEIGHT / 2);
        pong.ball.vel_y = -norm_rel_intersect * 0.5;
        
        pong.ball.vel_x *= 1.05;
        ball_speed_multiplier += 0.02;
        
        play_bounce_sound();
    }
    
    if (pong.ball.x <= 0) {
        pong.right_paddle.score++;
        
        ai_skill_bonus -= 0.1;
        if (ai_skill_bonus < 0) ai_skill_bonus = 0;
        
        play_score_sound();
        reset_ball();
    } else if (pong.ball.x >= PONG_WIDTH) {
        pong.left_paddle.score++;
        
        ai_skill_bonus += 0.1;
        
        play_score_sound();
        reset_ball();
    }
    
    float target_y = predict_ball_y_position();
    
    float base_paddle_speed = 0.3 * pong.difficulty;
    
    float adaptive_paddle_speed = base_paddle_speed * (1.0 + ai_skill_bonus);
    
    if (pong.ball.vel_x > 0) {
        if (pong.right_paddle.y + PADDLE_HEIGHT/2 < target_y - 0.5) {
            pong.right_paddle.y += adaptive_paddle_speed;
        } else if (pong.right_paddle.y + PADDLE_HEIGHT/2 > target_y + 0.5) {
            pong.right_paddle.y -= adaptive_paddle_speed;
        }
    } else {
        if (pong.right_paddle.y + PADDLE_HEIGHT/2 < PONG_HEIGHT/2 - 0.5) {
            pong.right_paddle.y += adaptive_paddle_speed * 0.5;
        } else if (pong.right_paddle.y + PADDLE_HEIGHT/2 > PONG_HEIGHT/2 + 0.5) {
            pong.right_paddle.y -= adaptive_paddle_speed * 0.5;
        }
    }
    
    if (pong.left_paddle.y < 1) pong.left_paddle.y = 1;
    if (pong.left_paddle.y > PONG_HEIGHT - PADDLE_HEIGHT - 1) 
        pong.left_paddle.y = PONG_HEIGHT - PADDLE_HEIGHT - 1;
    
    if (pong.right_paddle.y < 1) pong.right_paddle.y = 1;
    if (pong.right_paddle.y > PONG_HEIGHT - PADDLE_HEIGHT - 1) 
        pong.right_paddle.y = PONG_HEIGHT - PADDLE_HEIGHT - 1;
}

void handle_pong_input() {
    uint8_t scancode = inPortB(0x60);
    bool keyReleased = scancode & 0x80;
    scancode &= 0x7F; 
    
    if (!keyReleased) {
        last_scancode = scancode;
        
        switch (scancode) {
            case 0x48:
                pong.left_paddle.y -= 2;
                break;
            case 0x50:
                pong.left_paddle.y += 2;
                break;
            case 0x01:
                current_state = MENU_STATE_MAIN;
                break;
            case 0x39:
                pong.running = !pong.running;
                break;
            case 0x02:
                pong.difficulty = DIFFICULTY_EASY;
                break;
            case 0x03:
                pong.difficulty = DIFFICULTY_MEDIUM;
                break;
            case 0x04:
                pong.difficulty = DIFFICULTY_HARD;
                break;
            case 0x13:
                init_pong();
                break;
        }
    }
}

void render_pong() {
    Reset();
    
    setColor(UIA_WHITE, UIA_RED);
    for (uint16_t i = 0; top_border[i] != '\0' && i < PONG_WIDTH; i++) {
        putCharAt(i, 0, top_border[i], UIA_WHITE, UIA_RED);
    }
    
    for (uint16_t i = 0; bottom_border[i] != '\0' && i < PONG_WIDTH; i++) {
        putCharAt(i, PONG_HEIGHT, bottom_border[i], UIA_WHITE, UIA_RED);
    }
    
    for (uint16_t y = 1; y < PONG_HEIGHT; y++) {
        putCharAt(0, y, side_border[0], UIA_WHITE, UIA_RED);
        putCharAt(PONG_WIDTH, y, side_border[0], UIA_WHITE, UIA_RED);
    }
    
    setColor(WALL_COLOR, BG_COLOR);
    for (uint16_t y = 1; y < PONG_HEIGHT; y++) {
        if (y % 2 == 0) {
            putCharAt(PONG_WIDTH / 2, y, '|', WALL_COLOR, BG_COLOR);
        }
    }
    
    setColor(SCORE_COLOR, BG_COLOR);
    
    char left_score[4];
    itoa(pong.left_paddle.score, left_score, 10);
    putCharAt(PONG_WIDTH / 2 - 5, 1, '[', SCORE_COLOR, BG_COLOR);
    putCharAt(PONG_WIDTH / 2 - 3, 1, ']', SCORE_COLOR, BG_COLOR);
    for (uint16_t i = 0; left_score[i] != '\0'; i++) {
        putCharAt((PONG_WIDTH / 2 - 4) + i, 1, left_score[i], SCORE_COLOR, BG_COLOR);
    }
    
    char right_score[4];
    itoa(pong.right_paddle.score, right_score, 10);
    putCharAt(PONG_WIDTH / 2 + 3, 1, '[', SCORE_COLOR, BG_COLOR);
    putCharAt(PONG_WIDTH / 2 + 5, 1, ']', SCORE_COLOR, BG_COLOR);
    for (uint16_t i = 0; right_score[i] != '\0'; i++) {
        putCharAt((PONG_WIDTH / 2 + 4) + i, 1, right_score[i], SCORE_COLOR, BG_COLOR);
    }
    
    setColor(PADDLE_COLOR, BG_COLOR);
    for (uint16_t y = 0; y < PADDLE_HEIGHT; y++) {
        putCharAt(pong.left_paddle.x, (uint16_t)(pong.left_paddle.y + y), PADDLE_CHAR, PADDLE_COLOR, BG_COLOR);
    }
    
    for (uint16_t y = 0; y < PADDLE_HEIGHT; y++) {
        putCharAt(pong.right_paddle.x, (uint16_t)(pong.right_paddle.y + y), PADDLE_CHAR, PADDLE_COLOR, BG_COLOR);
    }
    
    setColor(BALL_COLOR, BG_COLOR);
    putCharAt((uint16_t)pong.ball.x, (uint16_t)pong.ball.y, BALL_CHAR, BALL_COLOR, BG_COLOR);
    
    if (!pong.running) {
        const char* paused_text = "PAUSED";
        int16_t pause_x = PONG_WIDTH / 2 - 3;
        int16_t pause_y = PONG_HEIGHT / 2;
        
        for (uint16_t i = 0; paused_text[i] != '\0'; i++) {
            putCharAt(pause_x + i, pause_y, paused_text[i], PAUSE_COLOR, BG_COLOR);
        }
    }
    
    setColor(TEXT_COLOR, BG_COLOR);
    const char* controls = "Controls: UP/DOWN=Move  1/2/3=Difficulty  SPACE=Pause  R=Reset  ESC=Menu";
    for (uint16_t i = 0; controls[i] != '\0' && i < PONG_WIDTH; i++) {
        putCharAt(i, PONG_HEIGHT + 1, controls[i], TEXT_COLOR, BG_COLOR);
    }
    
    const char* diff_text = "";
    switch (pong.difficulty) {
        case DIFFICULTY_EASY: diff_text = "Difficulty: Easy  "; break;
        case DIFFICULTY_MEDIUM: diff_text = "Difficulty: Medium"; break;
        case DIFFICULTY_HARD: diff_text = "Difficulty: Hard  "; break;
    }
    
    for (uint16_t i = 0; diff_text[i] != '\0'; i++) {
        putCharAt(i, PONG_HEIGHT + 2, diff_text[i], TEXT_COLOR, BG_COLOR);
    }
}

void pong_loop() {
    handle_pong_input();
    
    if (pong.running) {
        update_pong();
    }
    
    render_pong();
    
    if (current_state != MENU_STATE_PONG) {
        return;
    }
}