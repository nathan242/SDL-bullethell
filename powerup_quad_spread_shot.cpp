#include "powerup_quad_spread_shot.h"
#include "constants.h"

void powerup_quad_spread_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_QUAD_SPREAD_SHOT;

    animation = (animation_obj*)i_eng->get_resource("powerup_quad_spread_shot_anim");
}
