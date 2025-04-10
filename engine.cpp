#include "engine.h"

timer_obj::timer_obj(time_t init_every)
{
    every = init_every;
    last = {0, 0};
    suspended = false;
    suspend_time = {0, 0};
}

bool timer_obj::check(timespec now)
{
    int64_t timediff;

    if (suspended || every == 0) {
        return false;
    }

    timediff = ((now.tv_sec - last.tv_sec) * 1000000000) + (now.tv_nsec - last.tv_nsec);

    return timediff >= every;
}

bool timer_obj::tick(timespec now)
{
    if (check(now)) {
        last = now;
        return true;
    }

    return false;
}

void timer_obj::suspend(timespec now)
{
    if (suspended) {
        return;
    }

    suspended = true;
    suspend_time = now;
}

void timer_obj::resume(timespec now)
{
    if (!suspended) {
        return;
    }

    suspended = false;

    last.tv_nsec += (now.tv_nsec - suspend_time.tv_nsec);
    last.tv_sec += (now.tv_sec - suspend_time.tv_sec);

    if (last.tv_nsec > 1000000000) {
        last.tv_sec++;
        last.tv_nsec -= 1000000000;
    }
}

SDL_Texture* animation_obj::next(timespec now)
{
    if (timer->tick(now)) {
        if (++curr == frames.size()) {
            curr = 0;
        }
    }

    return frames[curr];
}

animation_obj::~animation_obj()
{
    if (timer != NULL) {
        delete timer;
    }
}

void engine_obj::init()
{
    type_id = 0;
    texture = NULL;
    animation = NULL;
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
    move_x = NULL;
    move_y = NULL;
    bounce = 0;
    collided = NULL;
    phys_active = false;
    draw_active = false;
    phys_collision_active = true;
    timer_list = NULL;
}

timer_obj* engine_obj::add_timer(time_t init_every)
{
    timer_obj_list *new_entry = new timer_obj_list;
    timer_obj_list *list;
    timer_obj *timer = new timer_obj(init_every);

    new_entry->obj = timer;
    new_entry->next = NULL;

    if (timer_list == NULL) {
        timer_list = new_entry;
    } else {
        list = timer_list;
        while (list->next != NULL) {
            list = list->next;
        }

        list->next = new_entry;
    }

    return timer;
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
    timer_obj_list *list = NULL;
    timer_obj_list *prev = NULL;

    if (timer_list != NULL) {
        list = timer_list;

        while (list != NULL) {
            prev = list;
            list = list->next;
            delete prev->obj;
            delete prev;
        }
    }
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

    update_timer();
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
    uint64_t timediff;
    int64_t timeremain;
    int iterations;
    bool run_loop = true;
    int loop_iterations = 0;

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

                if (obj->move_x == NULL || obj->move_x->every == 0 || obj->move_x->suspended) {
                    iterations = 0;
                } else if (obj->move_x->last.tv_sec == 0) {
                    iterations = 1;
                } else {
                    timediff = ((timer_now.tv_sec - obj->move_x->last.tv_sec) * 1000000000) + (timer_now.tv_nsec - obj->move_x->last.tv_nsec);
                    iterations = timediff / obj->move_x->every;
                    timeremain = timediff % obj->move_x->every;
                }

                if (iterations > 0) {
                    obj->pos_x += obj->step_x;

                    if (iterations > 1) {
                        run_loop = true;

                        obj->move_x->last.tv_nsec += obj->move_x->every;

                        if (obj->move_x->last.tv_nsec > 1000000000) {
                            obj->move_x->last.tv_sec++;
                            obj->move_x->last.tv_nsec -= 1000000000;
                        }
                    } else {
                        obj->move_x->last = timer_now;
                        if (timeremain > obj->move_x->last.tv_nsec) {
                            obj->move_x->last.tv_sec--;
                            obj->move_x->last.tv_nsec = 1000000000 - (timeremain - obj->move_x->last.tv_nsec);
                        } else {
                            obj->move_x->last.tv_nsec -= timeremain;
                        }
                    }
                }

                timeremain = 0;

                if (obj->move_y == NULL || obj->move_y->every == 0 || obj->move_y->suspended) {
                    iterations = 0;
                } else if (obj->move_y->last.tv_sec == 0) {
                    iterations = 1;
                } else {
                    timediff = ((timer_now.tv_sec - obj->move_y->last.tv_sec) * 1000000000) + (timer_now.tv_nsec - obj->move_y->last.tv_nsec);
                    iterations = timediff / obj->move_y->every;
                    timeremain = timediff % obj->move_y->every;
                }

                if (iterations > 0) {
                    obj->pos_y += obj->step_y;

                    if (iterations > 1) {
                        run_loop = true;

                        obj->move_y->last.tv_nsec += obj->move_y->every;

                        if (obj->move_y->last.tv_nsec > 1000000000) {
                            obj->move_y->last.tv_sec++;
                            obj->move_y->last.tv_nsec -= 1000000000;
                        }
                    } else {
                        obj->move_y->last = timer_now;
                        if (timeremain > obj->move_y->last.tv_nsec) {
                            obj->move_y->last.tv_sec--;
                            obj->move_y->last.tv_nsec = 1000000000 - (timeremain - obj->move_y->last.tv_nsec);
                        } else {
                            obj->move_y->last.tv_nsec -= timeremain;
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

void engine::draw()
{
    engine_obj_list *list = NULL;
    engine_obj *obj = NULL;

    list = list_head;

    SDL_RenderClear(renderer);

    while (list != NULL) {
        // Get object
        obj = list->obj;

        if (obj->draw_active) {
            if (obj->animation != NULL) {
                obj->texture = obj->animation->next(timer_now);
            }

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

void engine::update_timer()
{
    clock_gettime(CLOCK_MONOTONIC, &timer_now);
}

void engine::step()
{
    update_timer();
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

void engine::suspend_timers()
{
    engine_obj_list *obj_list = list_head;
    timer_obj_list *timer_list = NULL;

    while (obj_list != NULL) {
        if (obj_list->obj->timer_list != NULL) {
            timer_list = obj_list->obj->timer_list;

            while (timer_list != NULL) {
                timer_list->obj->suspend(timer_now);
                timer_list = timer_list->next;
            }
        }

        if (obj_list->obj->animation != NULL) {
            obj_list->obj->animation->timer->suspend(timer_now);
        }

        // Get next
        obj_list = obj_list->next;
    }
}

void engine::resume_timers()
{
    engine_obj_list *obj_list = list_head;
    timer_obj_list *timer_list = NULL;

    while (obj_list != NULL) {
        if (obj_list->obj->timer_list != NULL) {
            timer_list = obj_list->obj->timer_list;

            while (timer_list != NULL) {
                timer_list->obj->resume(timer_now);
                timer_list = timer_list->next;
            }
        }

        if (obj_list->obj->animation != NULL) {
            obj_list->obj->animation->timer->resume(timer_now);
        }

        // Get next
        obj_list = obj_list->next;
    }
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

