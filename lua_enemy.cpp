#include "lua_enemy.h"
#include "constants.h"

extern "C" {
    #include <lua5.4/lua.h>
    #include <lua5.4/lauxlib.h>
    #include <lua5.4/lualib.h>
}

extern lua_State *L;

int lua_obj_getter(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    const char *key = luaL_checkstring(L, 2);

    if (strcmp(key, "pos_x") == 0) {
        lua_pushnumber(L, lua_obj->pos_x);
        return 1;
    }

    if (strcmp(key, "pos_y") == 0) {
        lua_pushnumber(L, lua_obj->pos_y);
        return 1;
    }

    if (strcmp(key, "size_x") == 0) {
        lua_pushnumber(L, lua_obj->size_x);
        return 1;
    }

    if (strcmp(key, "size_y") == 0) {
        lua_pushnumber(L, lua_obj->size_y);
        return 1;
    }

    if (strcmp(key, "phys_size_x") == 0) {
        lua_pushnumber(L, lua_obj->phys_size_x);
        return 1;
    }

    if (strcmp(key, "phys_size_y") == 0) {
        lua_pushnumber(L, lua_obj->phys_size_y);
        return 1;
    }

    if (strcmp(key, "bounce") == 0) {
        lua_pushnumber(L, lua_obj->bounce);
        return 1;
    }

    if (strcmp(key, "default_health") == 0) {
        lua_pushnumber(L, lua_obj->default_health);
        return 1;
    }

    if (strcmp(key, "current_health") == 0) {
        lua_pushnumber(L, lua_obj->current_health);
        return 1;
    }

    luaL_getmetatable(L, "lua_enemy_meta");
    lua_pushstring(L, key);
    lua_rawget(L, -2);

    return 1;
}

int lua_obj_setter(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    const char *key = luaL_checkstring(L, 2);
    int value = luaL_checknumber(L, 3);

    if (strcmp(key, "pos_x") == 0) {
        lua_obj->pos_x = value;
        return 0;
    }

    if (strcmp(key, "pos_y") == 0) {
        lua_obj->pos_y = value;
        return 0;
    }

    if (strcmp(key, "size_x") == 0) {
        lua_obj->size_x = value;
        return 0;
    }

    if (strcmp(key, "size_y") == 0) {
        lua_obj->size_y = value;
        return 0;
    }

    if (strcmp(key, "phys_size_x") == 0) {
        lua_obj->phys_size_x = value;
        return 0;
    }

    if (strcmp(key, "phys_size_y") == 0) {
        lua_obj->phys_size_y = value;
        return 0;
    }

    if (strcmp(key, "bounce") == 0) {
        lua_obj->bounce = value;
        return 0;
    }

    if (strcmp(key, "default_health") == 0) {
        lua_obj->default_health = value;
        return 0;
    }

    if (strcmp(key, "current_health") == 0) {
        lua_obj->current_health = value;
        return 0;
    }

    return luaL_error(L, "Unknown field");
}

int lua_default_pre_phys_event(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    lua_obj->default_pre_phys_event();
    return 0;
}

int lua_add_timer(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    time_t every = luaL_checkinteger(L, 2);
    const char *field = luaL_checkstring(L, 3);

    timer_obj *timer = lua_obj->add_timer(every);

    if (strcmp(field, "move_x") == 0) {
        lua_obj->move_x = timer;
        return 0;
    }

    if (strcmp(field, "move_y") == 0) {
        lua_obj->move_y = timer;
        return 0;
    }

    if (strcmp(field, "shot_timer") == 0) {
        lua_obj->set_shot_timer(timer);
        return 0;
    }

    return luaL_error(L, "Unknown timer field");
}

int lua_set_texture(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    const char *texture_name = luaL_checkstring(L, 2);
    const char *field = luaL_checkstring(L, 3);

    SDL_Texture *texture = (SDL_Texture*)(lua_obj->get_engine()->get_resource(texture_name));

    if (texture == NULL) {
        return luaL_error(L, "Texture not found");
    }

    if (strcmp(field, "default_texture") == 0) {
        lua_obj->set_default_texture(texture);
        return 0;
    }

    if (strcmp(field, "hit_texture") == 0) {
        lua_obj->set_hit_texture(texture);
        return 0;
    }

    if (strcmp(field, "default_shot_texture") == 0) {
        lua_obj->set_shot_texture(texture);
        return 0;
    }

    return luaL_error(L, "Unknown texture field");
}

int lua_set_sfx(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    const char *sfx_name = luaL_checkstring(L, 2);
    const char *field = luaL_checkstring(L, 3);

    Mix_Chunk *sfx = (Mix_Chunk*)(lua_obj->get_engine()->get_resource(sfx_name));

    if (sfx == NULL) {
        return luaL_error(L, "Sfx not found");
    }

    if (strcmp(field, "default_shot_sfx") == 0) {
        lua_obj->set_default_shot_sfx(sfx);
        return 0;
    }

    return luaL_error(L, "Unknown sfx field");
}

int lua_fire(lua_State *L)
{
    lua_enemy *lua_obj = *(lua_enemy**)luaL_checkudata(L, 1, "lua_enemy_meta");
    int p_size_x = luaL_checkinteger(L, 2);
    int p_size_y = luaL_checkinteger(L, 3);
    int p_phys_size_x = luaL_checkinteger(L, 4);
    int p_phys_size_y = luaL_checkinteger(L, 5);
    int p_pos_x = luaL_checkinteger(L, 6);
    int p_pos_y = luaL_checkinteger(L, 7);
    int p_step_x = luaL_checkinteger(L, 8);
    int p_step_y = luaL_checkinteger(L, 9);
    int p_move_x_every = luaL_checkinteger(L, 10);
    int p_move_y_every = luaL_checkinteger(L, 11);
    bool p_invincible = lua_toboolean(L, 12);

    lua_obj->get_p_mngr()->fire(
        lua_obj->get_shot_texture(),
        p_size_x,
        p_size_y,
        p_phys_size_x,
        p_phys_size_y,
        p_pos_x,
        p_pos_y,
        p_step_x,
        p_step_y,
        p_move_x_every,
        p_move_y_every,
        p_invincible
    );

    return 0;
}

void lua_enemy::lua_init(const char *name)
{
    lua_obj_name = name;

    luaL_newmetatable(L, "lua_enemy_meta");

    lua_pushcfunction(L, lua_default_pre_phys_event);
    lua_setfield(L, -2, "default_pre_phys_event");
    lua_pushcfunction(L, lua_add_timer);
    lua_setfield(L, -2, "add_timer");
    lua_pushcfunction(L, lua_set_texture);
    lua_setfield(L, -2, "set_texture");
    lua_pushcfunction(L, lua_set_sfx);
    lua_setfield(L, -2, "set_sfx");
    lua_pushcfunction(L, lua_fire);
    lua_setfield(L, -2, "fire");

    lua_pushcfunction(L, lua_obj_getter);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, lua_obj_setter);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L, 1);

    lua_getglobal(L, name);
    lua_getfield(L, -1, "new");
    lua_pushvalue(L, -2);

    lua_enemy **udata = (lua_enemy**)lua_newuserdata(L, sizeof(lua_enemy*));
    *udata = this;
    luaL_getmetatable(L, "lua_enemy_meta");
    lua_setmetatable(L, -2);

    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        fprintf(stderr, "Error initializing LUA enemy object %s: %s\n", name, lua_tostring(L, -1));
        // TODO: Fail?
    }

    lua_obj_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);
}

void lua_enemy::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    // size_x = 21;
    // size_y = 31;
    // phys_size_x = 21;
    // phys_size_y = 31;
    // area_y_offset = 31;
    // move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    // move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    // bounce = 1;
    // default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_default_tex");

    // hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_default_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    // default_health = 2;
    // current_health = 2;

    // shot_timer = add_timer(800000000);


    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_obj_ref);
    lua_getfield(L, -1, "init");
    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error calling init for LUA enemy object %s: %s\n", lua_obj_name, lua_tostring(L, -1));
        // TODO: Fail?
    }

    lua_pop(L, 1);

    initialized = true;
}

void lua_enemy::fire()
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_obj_ref);
    lua_getfield(L, -1, "fire");
    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error calling fire for LUA enemy object %s: %s\n", lua_obj_name, lua_tostring(L, -1));
        // TODO: Fail?
    }

    lua_pop(L, 1);
}

void lua_enemy::pre_phys_event()
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_obj_ref);
    lua_getfield(L, -1, "pre_phys_event");
    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error calling pre_phys_event for LUA enemy object %s: %s\n", lua_obj_name, lua_tostring(L, -1));
        // TODO: Fail?
    }

    lua_pop(L, 1);
}

void lua_enemy::default_pre_phys_event()
{
    base_enemy::pre_phys_event();
}

void lua_enemy::init_projectile()
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, lua_obj_ref);
    lua_getfield(L, -1, "init_projectile");
    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        fprintf(stderr, "Error calling init_projectile for LUA enemy object %s: %s\n", lua_obj_name, lua_tostring(L, -1));
        // TODO: Fail?
    }

    lua_pop(L, 1);
}

engine* lua_enemy::get_engine()
{
    return i_eng;
}

projectile_manager* lua_enemy::get_p_mngr()
{
    return p_mngr;
}

void lua_enemy::set_shot_timer(timer_obj *timer)
{
    shot_timer = timer;
}

void lua_enemy::set_default_texture(SDL_Texture *texture)
{
    default_texture = texture;
}

void lua_enemy::set_hit_texture(SDL_Texture *texture)
{
    hit_texture = texture;
}

void lua_enemy::set_shot_texture(SDL_Texture *texture)
{
    default_shot_texture = texture;
}

SDL_Texture* lua_enemy::get_shot_texture()
{
    return default_shot_texture;
}

void lua_enemy::set_default_shot_sfx(Mix_Chunk *sfx)
{
    default_shot_sfx = sfx;
}

lua_enemy::~lua_enemy()
{
    if (initialized) {

    }
}
