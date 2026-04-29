--[[
    Sample Lua script

    Enemy types:
        enemy
        enemy_adv
        enemy_adv_spread
        enemy_allsprd_attacker
        enemy_boss_a
        enemy_boss_b
        enemy_boss_c
        enemy_cargo
        enemy_diagonal
        enemy_diagonal_stationary_allsprd
        enemy_diagonal_stationary
        enemy_diagonal_stationary_fwdsprd
        enemy_diagonal_stationary_spiral
        enemy_exploder

    Powerup types:
        double_shot
        quad_shot

    Background objects:
        background_1_tex - 800x2048
        background_2_tex - 1024x1536
        background_3_tex - 1024x2048

    Music objects:
        cosmic_annihilation_mus
        celestial_carnage_1_mus
        celestial_carnage_2_mus
        celestial_carnage_3_mus
        voidfire_descent_2_mus

    -------

    Methods:
        set_res(x, y) - Set game resolution (from init function)
        set_num_shots(x) - Set pool size for player shots
        set_num_shots_enemy(x) - Set pool size for enemy shots
        set_num_enemy_slots(x) - Set number of enemy slots
        set_num_explosions(x) - Set pool size for explosions
        init_level(level_no, background_object, size_x, size_y) - Init level of number with specified background
        set_music(music_object) - Set BGM
        reset_ship([optional] bool) - Reset ship to starting position, pass optional param as true to do a full reset (remove powerups)
        next_level() - Move to next level (active_level+1)
        complete() - Show complete screen
        create_enemy(type, pos_x, pos_y, step_x, step_y, powerup) - Create enemy
        create_custom_enemy(class, pos_x, pos_y, step_x, step_y, powerup) - Create custom enemy from Lua class
        add_texture(name, path) - Add custom texture
        add_music(name, path) - Add custom music
        add_sfx(name, path) - Add custom SFX
]]


-- Test enemy

test = {}
test.__index = test

function test:new(entity)
    local obj = {entity = entity}
    setmetatable(obj, self)
    return obj
end

function test:init()
    -- self.entity.pos_x = 200
    -- self.entity.pos_y = 300
    self.entity.size_x = 100
    self.entity.size_y = 100
    self.entity.phys_size_x = 100
    self.entity.phys_size_y = 100
    self.entity.area_y_offset = 100
    self.entity.bounce = 1
    self.entity.default_health = 5
    self.entity.current_health = 5

    self.entity:add_timer(12000000, "move_x")
    self.entity:add_timer(12000000, "move_y")
    self.entity:add_timer(800000000, "shot_timer")

    self.entity:set_texture("enemy_ship_default_tex", "default_texture")
    self.entity:set_texture("enemy_ship_default_hit_tex", "hit_texture")
end

function test:fire()
    self.entity:fire(10, 20, 10, 20, self.entity.pos_x+(self.entity.size_x/2)-2, self.entity.pos_y+self.entity.size_y+1, 0, 1, 2500000, 2500000, false)
end

function test:pre_phys_event()
    self.entity:default_pre_phys_event()
end

function test:init_projectile()
    self.entity:set_texture("projectile_default_tex", "default_shot_texture")
    self.entity:set_sfx("default_player_shot_snd", "default_shot_sfx")
end


-- Test enemy with custom texture

test_custom = {}
test_custom.__index = test_custom

function test_custom:new(entity)
    local obj = {entity = entity}
    setmetatable(obj, self)
    return obj
end

function test_custom:init()
    -- self.entity.pos_x = 200
    -- self.entity.pos_y = 300
    self.entity.size_x = 20
    self.entity.size_y = 20
    self.entity.phys_size_x = 20
    self.entity.phys_size_y = 20
    self.entity.area_y_offset = 20
    self.entity.bounce = 1
    self.entity.default_health = 10
    self.entity.current_health = 10

    self.entity:add_timer(18000000, "move_x")
    self.entity:add_timer(18000000, "move_y")
    self.entity:add_timer(800000000, "shot_timer")

    self.entity:set_texture("enemy_ship_test_tex", "default_texture")
    self.entity:set_texture("enemy_ship_test_hit_tex", "hit_texture")
end

function test_custom:fire()
    self.entity:fire(5, 10, 5, 10, self.entity.pos_x+(self.entity.size_x/2)-6, self.entity.pos_y+self.entity.size_y+1, 0, 1, 2500000, 2500000, false)
    self.entity:fire(5, 10, 5, 10, self.entity.pos_x+(self.entity.size_x/2)+5, self.entity.pos_y+self.entity.size_y+1, 0, 1, 2500000, 2500000, false)
end

function test_custom:pre_phys_event()
    self.entity:default_pre_phys_event()
end

function test_custom:init_projectile()
    self.entity:set_texture("projectile_default_tex", "default_shot_texture")
    self.entity:set_sfx("default_player_shot_snd", "default_shot_sfx")
end


-- Test enemy with custom firing

test_custom_fire = {}
test_custom_fire.__index = test_custom_fire

function test_custom_fire:new(entity)
    local obj = {entity = entity}
    setmetatable(obj, self)
    return obj
end

function test_custom_fire:init()
    -- self.entity.pos_x = 200
    -- self.entity.pos_y = 300
    self.entity.size_x = 20
    self.entity.size_y = 20
    self.entity.phys_size_x = 20
    self.entity.phys_size_y = 20
    self.entity.area_y_offset = 20
    self.entity.bounce = 1
    self.entity.default_health = 10
    self.entity.current_health = 10

    self.entity:add_timer(18000000, "move_x")
    self.entity:add_timer(18000000, "move_y")
    self.entity:add_timer(50000000, "shot_timer")

    self.entity:set_texture("enemy_ship_test_tex", "default_texture")
    self.entity:set_texture("enemy_ship_test_hit_tex", "hit_texture")
end

function test_custom_fire:fire()
    move_shot_x_every = 5/math.cos(self.ball_shot_angle*(math.pi/180))*1000000
    move_shot_y_every = 5/math.sin(self.ball_shot_angle*(math.pi/180))*1000000

    self.ball_shot_angle = self.ball_shot_angle + self.ball_shot_angle_diff

    if self.ball_shot_angle > 90 or self.ball_shot_angle < 0 then
        self.ball_shot_angle_diff = self.ball_shot_angle_diff * -1
        self.ball_shot_angle = self.ball_shot_angle + (self.ball_shot_angle_diff*2)

        if self.ball_fire_step_x == 1 and self.ball_fire_step_y == 1 then
            self.ball_fire_step_x = self.ball_fire_step_x*-1
        elseif self.ball_fire_step_x == -1 and self.ball_fire_step_y == 1 then
            self.ball_fire_step_y = self.ball_fire_step_y*-1
        elseif self.ball_fire_step_x == -1 and self.ball_fire_step_y == -1 then
            self.ball_fire_step_x = self.ball_fire_step_x*-1
        elseif self.ball_fire_step_x == 1 and self.ball_fire_step_y == -1 then
            self.ball_fire_step_y = self.ball_fire_step_y*-1
        end
    end

    self.entity:fire(
        20,
        20,
        20,
        20,
        self.entity.pos_x+(self.entity.size_x/2)-10,
        self.entity.pos_y+(self.entity.size_y/2)-10,
        self.ball_fire_step_x,
        self.ball_fire_step_y,
        math.floor(move_shot_x_every),
        math.floor(move_shot_y_every),
        false
    )
end

function test_custom_fire:pre_phys_event()
    self.entity:default_pre_phys_event()
end

function test_custom_fire:init_projectile()
    self.entity:set_texture("projectile_default_tex", "default_shot_texture")
    self.entity:set_sfx("default_player_shot_snd", "default_shot_sfx")
    self.ball_fire_step_x = 1
    self.ball_fire_step_y = 1
    self.ball_shot_angle = 0
    self.ball_shot_angle_diff = 10
end


function activate_enemy_set(active_level, active_enemy_set)
    if active_level == 0 then
        if active_enemy_set == 0 then
            init_level(1, "background_2_tex", 1024, 1536)
            set_music("celestial_carnage_1_mus")
        elseif active_enemy_set == 1 then
            reset_ship()
            create_custom_enemy("test", 100, -120, 1, 1, nil)
            create_custom_enemy("test", 300, -140, 1, 1, nil)
            -- create_enemy("enemy", 400, -40, 1, 1, nil)
            -- create_enemy("enemy_cargo", 300, -100, 0, 1, "quad_shot")
        elseif active_enemy_set == 2 then
            create_custom_enemy("test_custom", 100, -20, 1, 1, nil)
        elseif active_enemy_set == 3 then
            create_custom_enemy("test_custom_fire", 100, -20, 1, 1, "quad_shot")
        elseif active_enemy_set == 4 then
            create_enemy("enemy_diagonal", 70, -50, 1, 1, nil)
            create_enemy("enemy_diagonal", 450, -80, 1, 1, nil)
        elseif active_enemy_set == 5 then
            next_level()
        end
    elseif active_level == 1 then
        if active_enemy_set == 0 then
            init_level(2, "background_1_tex", 800, 2048)
            set_music("cosmic_annihilation_mus")
        elseif active_enemy_set == 2 then
            reset_ship()
            create_enemy("enemy_allsprd_attacker", 300, -100, 1, 1, nil)
        elseif active_enemy_set == 3 then
            complete()
        end
    end
end

function init()
    set_res(2560, 1600)
    set_num_shots(1000)
    set_num_shots_enemy(1000)
    set_num_enemy_slots(50)
    set_num_explosions(50)
    add_texture("enemy_ship_test_tex", "scripts/sample_textures/test_ship.png")
    add_texture("enemy_ship_test_hit_tex", "scripts/sample_textures/test_ship_hit.png")
end