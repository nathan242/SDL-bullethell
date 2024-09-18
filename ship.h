#ifndef SHIP_H
#define SHIP_H

#include "engine.h"
#include "projectile_manager.h"

class ship : public engine_obj
{
    protected:
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;

    public:
        ship(engine *eng, projectile_manager *projectile_mngr);
        void fire();
        ~ship();
};

#endif