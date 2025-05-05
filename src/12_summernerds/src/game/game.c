#include "kernel/pit.h"
#include "game/game.h"
#include "i386/keyboard.h"
#include "i386/monitor.h"
#include "random.h"

int fps = 12;
int objSprite = 61;
int score = 0;
// manages fps
void handle_fps(uint32_t frame_start)
{
    uint32_t current_tick = get_current_tick();
    uint32_t waitTime = 1000 / fps - (current_tick - frame_start);
    sleep_busy(waitTime);
}

typedef struct
{
    char type;
    int x;
    int y;

} object;

static object player;
static object objects[5];

void create_objects(object *objects)
{
    for (int i = 0; i < 5; i++)
    {
        objects[i].type = objSprite++;
        objects[i].x = randint(80);
        objects[i].y = randint(25);
    }
}

void draw_object(object obj)
{
    monitor_putentryat(obj.type, 3, obj.x, obj.y);
}

void updatePlayer()
{
    player.x += arrowKeys2D.x;
    player.y += arrowKeys2D.y;
    draw_object(player);
}

int runthegame()
{
    player.type = 'A';
    player.x = 40;
    player.y = 12;
    for (int i = 0; i < 5; i++)
    {
        objects[i].type = 0;
    }
    setupRNG(500);
    monitor_clear();
    printf("GAME TUTORIAL:\n"
           "You are the large letter\n"
           "Pick up small letters in order\n"
           "pick up the items to score points\n");
    sleep_interrupt(4000);
    while (1)
    {
        uint32_t startTick = get_current_tick();
        monitor_clear();
        printf("score: %d", score);
        updatePlayer();
        create_objects(objects);
        for (int i = 0; i < 5; i++)
        {
            object obj = objects[i];
            if (obj.x == player.x && obj.y == player.y)
            {
                objects[i].type = 0;
                score++;
            }
            else
                draw_object(obj);
        }

        if (has_user_pressed_esc())
            break;

        handle_fps(startTick);
    }
    return 0;
}