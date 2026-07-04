-- Test enemy

synth_fighter = {}
synth_fighter.__index = synth_fighter

function synth_fighter:new(entity)
    local obj = {entity = entity}
    setmetatable(obj, self)
    return obj
end

function synth_fighter:init()
    self.entity.size_x = 41
    self.entity.size_y = 40
    self.entity.phys_size_x = 41
    self.entity.phys_size_y = 40
    self.entity.area_y_offset = 40
    self.entity.bounce = 1
    self.entity.default_health = 10
    self.entity.current_health = 10

    self.entity:add_timer(12000000, "move_x")
    self.entity:add_timer(12000000, "move_y")
    self.entity:add_timer(800000000, "shot_timer")

    self.entity:set_texture("synth_fighter_tex", "default_texture")
    self.entity:set_texture("synth_fighter_hit_tex", "hit_texture")
end

function synth_fighter:fire()
    self.entity:fire(10, 20, 10, 20, self.entity.pos_x+self.entity.size_x+15, self.entity.pos_y+self.entity.size_y+1, 0, 1, 2500000, 2500000, false)
    self.entity:fire(10, 20, 10, 20, self.entity.pos_x+self.entity.size_x+25, self.entity.pos_y+self.entity.size_y+1, 0, 1, 2500000, 2500000, false)
end

function synth_fighter:pre_phys_event()
    self.entity:default_pre_phys_event()
end

function synth_fighter:init_projectile()
    self.entity:set_texture("projectile_synth_tex", "default_shot_texture")
    self.entity:set_sfx("default_player_shot_snd", "default_shot_sfx")
end


function activate_enemy_set(active_level, active_enemy_set)
    if active_level == 0 then
        if active_enemy_set == 0 then
            init_level(1, "background_synth_tex", 800, 600)
            set_music("synthwave_demo_mus")
        elseif active_enemy_set == 1 then
            reset_ship()
            create_custom_enemy("synth_fighter", 100, -40, 1, 1, nil)
            create_custom_enemy("synth_fighter", 200, -100, 1, 1, nil)
            create_custom_enemy("synth_fighter", 400, -200, 1, 1, nil)
        elseif active_enemy_set == 2 then
            create_custom_enemy("synth_fighter", 50, -40, 1, 1, nil)
            create_custom_enemy("synth_fighter", 250, -100, 1, 1, nil)
            create_custom_enemy("synth_fighter", 450, -200, 1, 1, nil)
        elseif active_enemy_set == 3 then
            create_custom_enemy("synth_fighter", 500, -40, 1, 1, nil)
            create_custom_enemy("synth_fighter", 400, -100, 1, 1, nil)
            create_custom_enemy("synth_fighter", 300, -200, 1, 1, nil)
            create_custom_enemy("synth_fighter", 200, -200, 1, 1, nil)
            create_custom_enemy("synth_fighter", 100, -200, 1, 1, nil)
        elseif active_enemy_set == 4 then
            create_custom_enemy("synth_fighter", 100, -40, 1, 1, nil)
            create_custom_enemy("synth_fighter", 200, -100, 1, 1, nil)
            create_custom_enemy("synth_fighter", 300, -200, 1, 1, nil)
            create_custom_enemy("synth_fighter", 400, -200, 1, 1, nil)
            create_custom_enemy("synth_fighter", 500, -200, 1, 1, nil)
        elseif active_enemy_set == 5 then
            complete()
        end
    end
end

function init()
    -- set_res(800, 600)
    -- set_num_shots(1000)
    -- set_num_shots_enemy(1000)
    -- set_num_enemy_slots(50)
    -- set_num_explosions(50)

    clear_texture_map()
    clear_music_map()
    clear_sfx_map()
    clear_texture_map()

    add_texture("background_1_tex", "textures/background_1.png")
    add_texture("menu_title_tex", "textures/title.png")
    add_texture("level_text_tex", "textures/level_text.png")
    add_texture("1_tex", "textures/1.png")
    add_texture("press_key_tex", "textures/press_a_key.png")
    add_texture("game_over_tex", "textures/game_over.png")
    add_texture("paused_tex", "textures/paused.png")
    add_texture("complete_tex", "textures/complete.png")
    add_texture("game_ui_bar_tex", "textures/game_ui_bar.png")
    add_texture("green_bar_tex", "textures/green_bar.png")
    add_texture("red_bar_tex", "textures/red_bar.png")
    add_texture("ship_tex", "textures/ship.png")
    add_texture("shield_tex", "textures/shield.png")
    add_texture("projectile_player_default_tex", "scripts/sample_textures/projectile_player_synth.png") -- override

    add_music("cosmic_annihilation_mus", "music/cosmic_annihilation.ogg")

    add_sfx("default_player_shot_snd", "sfx/default_player_shot.wav")
    add_sfx("explosion_snd", "sfx/explosion.wav")
    add_sfx("shield_snd", "sfx/shield.wav")
    add_sfx("laser_hit_snd", "sfx/laser_hit.wav")

    add_music("synthwave_demo_mus", "scripts/sample_music/synthwave_demo.ogg")
    add_music("voidfire_descent_2_mus", "music/voidfire_descent_2.ogg")

    add_texture("synth_fighter_tex", "scripts/sample_textures/synth_fighter.png")
    add_texture("synth_fighter_hit_tex", "scripts/sample_textures/synth_fighter_hit.png")
    add_texture("projectile_synth_tex", "scripts/sample_textures/synth_projectile.png")
    add_texture("background_synth_tex", "scripts/sample_textures/synth_background.png")

    add_texture("blue_explosion_1_tex", "scripts/sample_textures/explosion_1.png")
    add_texture("blue_explosion_2_tex", "scripts/sample_textures/explosion_2.png")
    add_texture("blue_explosion_3_tex", "scripts/sample_textures/explosion_3.png")
    add_texture("blue_explosion_4_tex", "scripts/sample_textures/explosion_4.png")

    add_animation(
        "explosion_anim",
        150000000,
        {
            "",
            "blue_explosion_1_tex",
            "blue_explosion_2_tex",
            "blue_explosion_3_tex",
            "blue_explosion_4_tex",
            "blue_explosion_1_tex"
        }
    )
end