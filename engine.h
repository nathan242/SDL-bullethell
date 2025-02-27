#ifndef ENGINE_H
#define ENGINE_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <time.h>

#include <SDL2/SDL.h>

class engine_obj
{
    public:
        int type_id;

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

        int area_x_offset;
        int area_y_offset;

        time_t move_x_every;
        time_t move_y_every;
        timespec move_x_last;
        timespec move_y_last;

        int bounce;
        engine_obj *collided;

        bool phys_active;
        bool draw_active;
        bool phys_collision_active;

        virtual void init();
        virtual bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        virtual void pre_phys_event();
        virtual ~engine_obj();
};

struct engine_obj_list
{
    int id;
    engine_obj *obj;
    engine_obj_list *next;
};

class engine
{
    protected:
        SDL_Rect offset;

        int list_len;

        engine_obj_list *list_head;
        engine_obj_list *list_curr;

        int area_x;
        int area_y;

        std::unordered_map<std::string, void *> resource_map;

        void check_collide(engine_obj *obj, int id);
    public:
        SDL_Window *window;
        SDL_Renderer *renderer;

        engine(const char* caption, int res_x, int res_y, int bpp, bool fullscreen = false);
        engine_obj_list* add_object(engine_obj *obj);
        void step();
        void draw();
        void phys_advance();
        void reset_phys_timings();
        void add_resource(const char *name, void *resource);
        void *get_resource(const char *name);
        ~engine();
};

#endif
