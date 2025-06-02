#include "explosion_manager.h"

explosion_manager::explosion_manager()
{
    list_head = NULL;
    list_curr = NULL;
}

void explosion_manager::add_object(explosion *obj)
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

void explosion_manager::explode(int size_x, int size_y, int pos_x, int pos_y)
{
    obj_list *list = NULL;
    explosion *obj = NULL;

    list = list_head;

    // Find avaliable explosion
    while (list != NULL) {
        obj = list->obj;

        if (obj->animation->frames[obj->animation->curr] == NULL) {
            obj->animation->curr = 1;

            obj->texture = obj->animation->frames[obj->animation->curr];
            obj->size_x = size_x;
            obj->size_y = size_y;
            obj->area_x_offset = size_x;
            obj->area_y_offset = size_y;
            obj->pos_x = pos_x;
            obj->pos_y = pos_y;

            break;
        }

        list = list->next;
    }
}

explosion_manager::~explosion_manager()
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
