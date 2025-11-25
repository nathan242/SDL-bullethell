function activate_enemy_set(active_level, active_enemy_set)
    if active_level == 0 then
        if active_enemy_set == 0 then
            init_level(1, "background_2_tex", 1024, 1536)
            set_music("cosmic_annihilation_mus")
        elseif active_enemy_set == 1 then
            reset_ship()
            create_enemy("enemy_boss_a", 350, -100, 0, 1, nil)
        elseif active_enemy_set == 2 then
            create_enemy("enemy_cargo", 300, -100, 0, 1, "double_shot")
        elseif active_enemy_set == 3 then
            create_enemy("enemy_boss_b", 350, -215, 0, 1, nil)
        elseif active_enemy_set == 4 then
            create_enemy("enemy_cargo", 300, -100, 0, 1, "quad_shot")
        elseif active_enemy_set == 5 then
            create_enemy("enemy_boss_c", 350, -181, 0, 1, nil)
        elseif active_enemy_set == 6 then
            complete()
        end
    end
end
