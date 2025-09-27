#include "level_text.h"
#include "constants.h"

level_text::level_text(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void level_text::init()
{
    engine_obj::init();

    type_id = ID_LEVEL_TEXT;
    pos_y = 291;
    size_x = 65;
    size_y = 18;
    move_x = add_timer(2000000);
    
    texture = (SDL_Texture*)i_eng->get_resource("level_text_tex");
    numbers[0] = (SDL_Texture*)i_eng->get_resource("1_tex");
    numbers[1] = (SDL_Texture*)i_eng->get_resource("2_tex");
    numbers[2] = (SDL_Texture*)i_eng->get_resource("3_tex");

    wait_timer = add_timer(2000000000);
    show_number = 0;
    step = 0;

    phys_collision_active = false;

    initialized = true;
}

void level_text::set(int level)
{
    show_number = level;
    pos_x = 0;
    step_x = 1;
    step = 0;
    draw_active = true;
    phys_active = true;
}

void level_text::pre_phys_event()
{
    switch (step) {
        case 0:
            if (pos_x == 358) {
                step++;

                wait_timer->last = i_eng->timer_now;
                step_x = 0;
            }
            break;

        case 1:
            if (wait_timer->tick(i_eng->timer_now)) {
                step++;
                step_x = 1;
            }
            break;

        case 2:
            if (pos_x == 800) {
                draw_active = false;
                phys_active = false;
            }
            break;
    }
}

void level_text::post_draw_event()
{
    offset.x = pos_x+size_x+10;
    offset.y = pos_y;
    offset.w = 12;
    offset.h = 19;

    SDL_RenderCopy(i_eng->renderer, numbers[show_number-1], NULL, &offset);
}

level_text::~level_text()
{
    if (initialized) {
        
    }
}
