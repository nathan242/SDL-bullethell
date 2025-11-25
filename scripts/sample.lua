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
        init_level(level_no, background_object, size_x, size_y) - Init level of number with specified background
        set_music(music_object) - Set BGM
        reset_ship([optional] bool) - Reset ship to starting position, pass optional param as true to do a full reset (remove powerups)
        next_level() - Move to next level (active_level+1)
        complete() - Show complete screen
]]

function activate_enemy_set(active_level, active_enemy_set)
    if active_level == 0 then
        if active_enemy_set == 0 then
            init_level(1, "background_2_tex", 1024, 1536)
            set_music("celestial_carnage_1_mus")
        elseif active_enemy_set == 1 then
            reset_ship()
            create_enemy("enemy", 100, -20, 1, 1, nil)
            create_enemy("enemy", 400, -40, 1, 1, nil)
            create_enemy("enemy_cargo", 300, -100, 0, 1, "quad_shot")
        elseif active_enemy_set == 2 then
            create_enemy("enemy_diagonal", 70, -50, 1, 1, nil)
            create_enemy("enemy_diagonal", 450, -80, 1, 1, nil)
        elseif active_enemy_set == 3 then
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
