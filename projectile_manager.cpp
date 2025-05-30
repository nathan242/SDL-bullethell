#include "projectile_manager.h"

projectile_manager::projectile_manager()
{
    list_head = NULL;
    list_curr = NULL;
}

void projectile_manager::add_object(base_projectile *obj)
{
    obj_list *list = new obj_list;
    list->obj = obj;
    list->next = NULL;

    // First item
    if (list_head == NULL) {
        list_head = list;
        list_curr = list;
    } else {
        // Append to list
        list_curr->next = list;
        list_curr = list;
    }
}

void projectile_manager::fire(
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
    int move_y_every,
    bool invincible
)
{
    obj_list *list = NULL;
    base_projectile *obj = NULL;

    list = list_head;

    // Find avaliable projectile
    while (list != NULL) {
        obj = list->obj;

        if (!obj->draw_active) {
            obj->texture = texture;
            obj->animation = NULL;
            obj->size_x = size_x;
            obj->size_y = size_y;
            obj->area_x_offset = size_x;
            obj->area_y_offset = size_y;
            obj->phys_size_x = phys_size_x;
            obj->phys_size_y = phys_size_y;
            obj->pos_x = pos_x;
            obj->pos_y = pos_y;
            obj->step_x = step_x;
            obj->step_y = step_y;
            obj->move_x->every = move_x_every;
            obj->move_y->every = move_y_every;
            obj->move_x->last = {0, 0};
            obj->move_y->last = {0, 0};
            obj->invincible = invincible;

            obj->draw_active = true;
            obj->phys_active = true;

            break;
        }

        list = list->next;
    }
}

void projectile_manager::fire(
    animation_obj *animation,
    int size_x,
    int size_y,
    int phys_size_x,
    int phys_size_y,
    int pos_x,
    int pos_y,
    int step_x,
    int step_y,
    int move_x_every,
    int move_y_every,
    bool invincible
)
{
    obj_list *list = NULL;
    base_projectile *obj = NULL;

    list = list_head;

    // Find avaliable projectile
    while (list != NULL) {
        obj = list->obj;

        if (!obj->draw_active) {
            obj->animation = animation;
            obj->texture = animation->frames[animation->curr];
            obj->size_x = size_x;
            obj->size_y = size_y;
            obj->area_x_offset = size_x;
            obj->area_y_offset = size_y;
            obj->phys_size_x = phys_size_x;
            obj->phys_size_y = phys_size_y;
            obj->pos_x = pos_x;
            obj->pos_y = pos_y;
            obj->step_x = step_x;
            obj->step_y = step_y;
            obj->move_x->every = move_x_every;
            obj->move_y->every = move_y_every;
            obj->move_x->last = {0, 0};
            obj->move_y->last = {0, 0};
            obj->invincible = invincible;

            obj->draw_active = true;
            obj->phys_active = true;

            break;
        }

        list = list->next;
    }
}

projectile_manager::~projectile_manager()
{
    if (list_head != NULL) {
        obj_list *list = NULL;
        obj_list *prev = NULL;

        list = list_head;

        while (list != NULL) {
            prev = list;
            list = list->next;
            delete prev;
        }
    }
}
