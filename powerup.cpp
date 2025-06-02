#include "powerup.h"
#include "constants.h"

powerup::powerup(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void powerup::init()
{
    engine_obj::init();

    size_x = 21;
    size_y = 21;
    phys_size_x = 21;
    phys_size_y = 21;
    area_x_offset = 21;
    area_y_offset = 21;
    move_y = add_timer(BACKGROUND_SCROLL_EVERY);
    step_y = 1;
    bounce = -1;

    initialized = true;
}

bool powerup::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 == NULL) {
        draw_active = false;
        phys_active = false;
    }

    return false;
}

powerup::~powerup()
{
    if (initialized) {
        
    }
}
