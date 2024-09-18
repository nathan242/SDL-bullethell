#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "engine.h"
#include <SDL2/SDL.h>

class projectile_manager
{
    protected:
        struct obj_list
        {
            engine_obj *obj;
            obj_list *next;
        };

        obj_list *list_head;
        obj_list *list_curr;
    
    public:
        projectile_manager();
        void add_object(engine_obj *obj);
        void fire(
            SDL_Texture *texture,
            int size_x,
            int size_y,
            int phys_size_x,
            int phys_size_y,
            int pos_x,
            int pos_y,
            int step_x,
            int step_y,
            int move_x_every,
            int move_y_every
        );
};

#endif
