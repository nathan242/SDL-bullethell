#ifndef ENGINE_H
#define ENGINE_H

#include <cstddef>
#include <cstdint>
#include <time.h>

#include <SDL2/SDL.h>

class engine_obj
{
    public:
        int type_id; // Needed?

        SDL_Surface *sprite;
        SDL_Texture *texture;

        int size_x;
        int size_y;

        int phys_size_x;
        int phys_size_y;

        int pos_x;
        int pos_y;

        int step_x;
        int step_y;

        time_t move_x_every;
        time_t move_y_every;
        timespec move_x_last;
        timespec move_y_last;

        int bounce;
        engine_obj *collided;

        void (*callback)(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y);

        bool phys_active;
        bool draw_active;
};

class engine
{
    private:
        SDL_Rect offset;

        int list_len;

        struct obj_list
        {
            int id;
            engine_obj *obj;
            obj_list *next;
        };

        obj_list *list_head;
        obj_list *list_curr;

        int area_x;
        int area_y;

        void check_collide(engine_obj *obj, int id);
    public:
        SDL_Window *window;
        SDL_Renderer *renderer;

        engine(const char* caption, int res_x, int res_y, int bpp);
        int add_object(engine_obj *obj);
        void step();
        void draw();
        void phys_advance();
        void check_collide();
        void reset_phys_timings();
        ~engine();
};

#endif
