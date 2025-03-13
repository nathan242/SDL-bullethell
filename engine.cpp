#include "engine.h"

void engine_obj::init()
{
    type_id = 0;
    sprite = NULL;
    texture = NULL;
    size_x = 0;
    size_y = 0;
    phys_size_x = 0;
    phys_size_y = 0;
    pos_x = 0;
    pos_y = 0;
    step_x = 0;
    step_y = 0;
    area_x_offset = 0;
    area_y_offset = 0;
    move_x_every = 0;
    move_y_every = 0;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = 0;
    collided = NULL;
    phys_active = false;
    draw_active = false;
    phys_collision_active = true;
}

bool engine_obj::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    return true;
}

void engine_obj::pre_phys_event()
{

}

engine_obj::~engine_obj()
{

}

engine::engine(const char* caption, int res_x, int res_y, int bpp, bool fullscreen)
{
    Uint32 window_flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;

    area_x = res_x;
    area_y = res_y;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, res_x, res_y, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (fullscreen) {
        SDL_RenderSetLogicalSize(renderer, res_x, res_y);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    list_len = 0;
    list_head = NULL;
    list_curr = NULL;

    phys_max_iterations = 0;
}

engine_obj_list* engine::add_object(engine_obj *obj)
{
    engine_obj_list *list = new engine_obj_list;
    list->id = list_len++;
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

    return list;
}

void engine::phys_advance()
{
    engine_obj_list *list = NULL;
    engine_obj *obj = NULL;
    timespec now;
    uint64_t timediff;
    int64_t timeremain;
    int iterations;
    bool run_loop = true;
    int loop_iterations = 0;

    clock_gettime(CLOCK_MONOTONIC, &now);

    while (run_loop) {
        run_loop = false;

        list = list_head;

        while (list != NULL) {
            // Get object
            obj = list->obj;

            if (obj->phys_active) {
                obj->pre_phys_event();

                if (obj->phys_collision_active) {
                    // Check if object is colliding with another
                    check_collide(obj, list->id);
                }
            }

            // Get next
            list = list->next;
        }

        list = list_head;

        while (list != NULL) {
            // Get object
            obj = list->obj;

            if (obj->phys_active) {
                timeremain = 0;

                if (obj->move_x_every == 0) {
                    iterations = 0;
                } else if (obj->move_x_last.tv_sec == 0) {
                    iterations = 1;
                } else {
                    timediff = ((now.tv_sec - obj->move_x_last.tv_sec) * 1000000000) + (now.tv_nsec - obj->move_x_last.tv_nsec);
                    iterations = timediff / obj->move_x_every;
                    timeremain = timediff % obj->move_x_every;
                }

                if (iterations > 0) {
                    obj->pos_x += obj->step_x;

                    if (iterations > 1) {
                        run_loop = true;

                        obj->move_x_last.tv_nsec += obj->move_x_every;

                        if (obj->move_x_last.tv_nsec > 1000000000) {
                            obj->move_x_last.tv_sec++;
                            obj->move_x_last.tv_nsec -= 1000000000;
                        }
                    } else {
                        obj->move_x_last = now;
                        if (timeremain > obj->move_x_last.tv_nsec) {
                            obj->move_x_last.tv_sec--;
                            obj->move_x_last.tv_nsec = 1000000000 - (timeremain - obj->move_x_last.tv_nsec);
                        } else {
                            obj->move_x_last.tv_nsec -= timeremain;
                        }
                    }
                }

                timeremain = 0;

                if (obj->move_y_every == 0) {
                    iterations = 0;
                } else if (obj->move_y_last.tv_sec == 0) {
                    iterations = 1;
                } else {
                    timediff = ((now.tv_sec - obj->move_y_last.tv_sec) * 1000000000) + (now.tv_nsec - obj->move_y_last.tv_nsec);
                    iterations = timediff / obj->move_y_every;
                    timeremain = timediff % obj->move_y_every;
                }

                if (iterations > 0) {
                    obj->pos_y += obj->step_y;

                    if (iterations > 1) {
                        run_loop = true;

                        obj->move_y_last.tv_nsec += obj->move_y_every;

                        if (obj->move_y_last.tv_nsec > 1000000000) {
                            obj->move_y_last.tv_sec++;
                            obj->move_y_last.tv_nsec -= 1000000000;
                        }
                    } else {
                        obj->move_y_last = now;
                        if (timeremain > obj->move_y_last.tv_nsec) {
                            obj->move_y_last.tv_sec--;
                            obj->move_y_last.tv_nsec = 1000000000 - (timeremain - obj->move_y_last.tv_nsec);
                        } else {
                            obj->move_y_last.tv_nsec -= timeremain;
                        }
                    }
                }
            }

            // Get next
            list = list->next;
        }

        if (phys_max_iterations > 0 && ++loop_iterations >= phys_max_iterations) {
            run_loop = false;
        }
    }
}

void engine::reset_phys_timings()
{
    engine_obj_list *list = list_head;
    timespec inittime {0, 0};

    while (list != NULL) {
        list->obj->move_x_last = inittime;
        list->obj->move_y_last = inittime;

        // Get next
        list = list->next;
    }
}

void engine::check_collide(engine_obj *obj, int id)
{
    engine_obj_list *list = NULL;
    engine_obj *obj2 = NULL;

    int x1;
    int x2;
    int y1;
    int y2;

    int diff_x;
    int diff_y;

    bool do_bounce;

    list = list_head;

    // Check collision with other objects
    while (list != NULL) {
        if (list->id != id && list->obj->phys_active && list->obj->phys_collision_active) {
            obj2 = list->obj;

            // Left side
            x1 = obj2->pos_x-obj->size_x;
            // Right side
            x2 = obj2->pos_x+obj2->size_x;
            // Top side
            y1 = obj2->pos_y-obj->size_y;
            // Bottom side
            y2 = obj2->pos_y+obj2->size_y;

            if (obj->pos_x >= x1 && obj->pos_x <= x2 && obj->pos_y >= y1 && obj->pos_y <= y2) {
                if (obj->collided != obj2) {
                    if (obj->pos_x-x1 > x2-obj->pos_x) {
                        // Collided on right side
                        diff_x = x2-obj->pos_x;
                    } else {
                        // Collided on left side
                        diff_x = obj->pos_x-x1;
                    }

                    if (obj->pos_y-y1 > y2-obj->pos_y) {
                        // Collided on bottom side
                        diff_y = y2-obj->pos_y;
                    } else {
                        // Collided on top side
                        diff_y = obj->pos_y-y1;
                    }

                    if (diff_y > diff_x) {
                        do_bounce = obj->collision_event(obj2, 1, area_x, area_y);
                        if (do_bounce) {
                            if (obj->bounce > 0) {
                                if ((obj->step_x > 0 && obj->pos_x < obj2->pos_x) || (obj->step_x < 0 && obj->pos_x > obj2->pos_x)) {
                                    obj->step_x = obj->step_x*-1;
                                }
                            } else if (obj->bounce == 0) {
                                obj->step_x = 0;
                                obj->step_y = 0;
                            }
                        }
                    } else {
                        do_bounce = obj->collision_event(obj2, 2, area_x, area_y);
                        if (do_bounce) {
                            if (obj->bounce > 0) {
                                if ((obj->step_y > 0 && obj->pos_y < obj2->pos_y) || (obj->step_y < 0 && obj->pos_y > obj2->pos_y)) {
                                    obj->step_y = obj->step_y*-1;
                                }
                            } else if (obj->bounce == 0) {
                                obj->step_x = 0;
                                obj->step_y = 0;
                            }
                        }
                    }
                    obj->collided = obj2;
                }
            } else if (obj->collided == obj2) {
                obj->collided = NULL;
            }
        }

        list = list->next;
    }

    // Check collision with edges
    if ((obj->pos_x >= (area_x+obj->area_x_offset)-obj->size_x && obj->step_x > 0) || (obj->pos_x <= (obj->area_x_offset*-1) && obj->step_x < 0)) {
        do_bounce = obj->collision_event(NULL, 1, area_x, area_y);
        if (do_bounce) {
            if (obj->bounce > 0) {
                obj->step_x = obj->step_x*-1;
            } else if (obj->bounce == 0 && ((obj->pos_x >= area_x-obj->size_x && obj->step_x > 0) || (obj->pos_x <= 0 && obj->step_x < 0))) {
                obj->step_x = 0;
                obj->step_y = 0;
            }
        }
    }
    if ((obj->pos_y >= (area_y+obj->area_y_offset)-obj->size_y && obj->step_y > 0) || (obj->pos_y <= (obj->area_y_offset*-1) && obj->step_y < 0)) {
        do_bounce = obj->collision_event(NULL, 2, area_x, area_y);
        if (do_bounce) {
            if (obj->bounce > 0) {
                obj->step_y = obj->step_y*-1;
            } else if (obj->bounce == 0 && ((obj->pos_y >= area_y-obj->size_y && obj->step_y > 0) || (obj->pos_y <= 0 && obj->step_y < 0))) {
                obj->step_x = 0;
                obj->step_y = 0;
            }
        }
    }
}

void engine::draw() {
    engine_obj_list *list = NULL;
    engine_obj *obj = NULL;

    list = list_head;

    SDL_RenderClear(renderer);

    while (list != NULL) {
        // Get object
        obj = list->obj;

        if (obj->draw_active) {
            offset.x = obj->pos_x;
            offset.y = obj->pos_y;
            offset.w = obj->size_x;
            offset.h = obj->size_y;

            SDL_RenderCopy(renderer, obj->texture, NULL, &offset);
        }

        // Get next
        list = list->next;
    }

    SDL_RenderPresent(renderer);
}

void engine::step()
{
    phys_advance();
    draw();
}

void engine::add_resource(const char *name, void *resource)
{
    resource_map[name] = resource;
}

void *engine::get_resource(const char *name)
{
    return resource_map[name];
}

engine::~engine()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (list_head != NULL) {
        engine_obj_list *list = NULL;
        engine_obj_list *prev = NULL;

        list = list_head;

        while (list != NULL) {
            prev = list;
            list = list->next;
            delete prev;
        }
    }
}

