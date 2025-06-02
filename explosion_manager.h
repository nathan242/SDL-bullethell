#ifndef EXPLOSION_MANAGER_H
#define EXPLOSION_MANAGER_H

#include "explosion.h"
#include <SDL2/SDL.h>

class explosion_manager
{
    protected:
        struct obj_list
        {
            explosion *obj;
            obj_list *next;
        };

        obj_list *list_head;
        obj_list *list_curr;

    public:
        explosion_manager();
        void add_object(explosion *obj);
        void explode(int size_x, int size_y, int pos_x, int pos_y);
        ~explosion_manager();
};

#endif
