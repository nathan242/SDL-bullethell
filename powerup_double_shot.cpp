#include "powerup_double_shot.h"

extern int ID_POWERUP_DOUBLE_SHOT;

void powerup_double_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_DOUBLE_SHOT;

    animation = (animation_obj*)i_eng->get_resource("powerup_double_shot_anim");
}
