#include "tetris/tetris.hpp"


ttr::TetrisManager::TetrisManager(){}

ttr::TetrisManager::TetrisManager(YAML::Node* config_, sf::RenderWindow* window_, ttr::InputManager* inputmanager){
    config = config_;
    window = window_;
    grid_lines = ttr::load_grid(*config, window);

    sprite_sheet.loadFromFile((*config)["Sheet"]["name"].as<std::string>());

    grid_width = grid_lines.get_width();
    grid_height = grid_lines.get_height();
    square_size = grid_lines.get_square_size();
    square_scale = (*config)["Sheet"]["scale"].as<int>();
    grid = std::vector<std::vector<bool>>(grid_height, std::vector<bool>(grid_width, false));
    display_grid = std::vector<std::vector<ttr::BlockType>>(grid_height, std::vector<ttr::BlockType>(grid_width, ttr::BlockType::Empty));

    tick_interval = (*config)["Game"]["tick_interval"].as<float>();
    tick_modifier = (*config)["Game"]["interval_modifier"].as<float>();

    sprites = ttr::load_sprites((*config)["Sheet"], sprite_sheet);
    shadow_sprites = ttr::load_sprites((*config)["Shadow"], sprite_sheet);

    clock = ttr::GameClock(tick_interval);
    inputmgr = inputmanager;
    inputClock = ttr::GameClock((*config)["Game"]["input_clock"].as<float>());

    game_state = -1;

    current_block = nullptr;
    holded = ttr::BlockType::Empty;
    block_pouch = ttr::Pouch();
    gameui = ttr::NextAndHold(window, block_pouch.get_next_list(), &holded, &score, &game_state, *config, &sprite_sheet);
    spawn_block();

    hold_available = true;
    block_movement = false;
    shadow_enabled = (*config)["Game"]["enable_shadow"].as<int>();
    score = 0;

    bgm.openFromFile((*config)["Audio"]["bgm"].as<std::string>());
    bgm.setLoop(true);
    bgm.setVolume(65);
    hard_drop.loadFromFile((*config)["Audio"]["hard_drop"].as<std::string>());
    soft_drop.loadFromFile((*config)["Audio"]["soft_drop"].as<std::string>());
    line_clear.loadFromFile((*config)["Audio"]["clear"].as<std::string>());
    
    hard_drop_sound = sf::Sound(hard_drop);
    soft_drop_sound = sf::Sound(soft_drop);
    line_clear_sound = sf::Sound(line_clear);
    line_clear_sound.setVolume(40);
}

ttr::TetrisManager::~TetrisManager(){
    if(current_block != nullptr){
        delete current_block;
    }
}

bool ttr::TetrisManager::set_block_at(ttr::BlockType btype, int x, int y){
    if(!(0 <= x && x < grid_width && 0 <= y && y < grid_height)) return false;
    // Can't place block if there's already one
    if(grid[y][x]) return false;

    grid[y][x] = true;
    display_grid[y][x] = btype;

    return true;
}

bool ttr::TetrisManager::remove_block_at(int x, int y){
    if(!(0 <= x && x < grid_width && 0 <= y && y < grid_height)) return false;
    if(!grid[y][x]) return false;

    grid[y][x] = false;
    display_grid[y][x] = ttr::BlockType::Empty;

    return true;
}

void ttr::TetrisManager::draw(){
    if(game_state != -1){
        for(int y = 0; y < grid_height; y++){
            for(int x = 0; x < grid_width; x++){
                sprites[display_grid[y][x]].setScale(sf::Vector2f(square_scale, square_scale));
                sprites[display_grid[y][x]].setPosition(grid_lines.get_position() + sf::Vector2f(x * square_size, y * square_size));
                window->draw(sprites[display_grid[y][x]]);
            }
        }
        current_block->draw(window, square_size, sprites, shadow_sprites, grid_lines.get_position(), square_scale);
        if(shadow_enabled){
            current_block->draw(window, square_size, sprites, shadow_sprites, grid_lines.get_position(), square_scale, true);
        }
    }
    grid_lines.draw();
    gameui.draw();
}

void ttr::TetrisManager::update(){
    clock.update();
    inputClock.update();

    if(inputmgr->is_pressed_down(sf::Keyboard::Enter) && game_state == -1){
        game_state = 0;
        bgm.play();
    }
    if(game_state != 0) return;

    // MOVEMENT STUFF
    if(inputClock.tick()){
        if(inputmgr->is_pressed(sf::Keyboard::Left) && !block_movement){
            current_block->move_horizontal(false);
        }
        if(inputmgr->is_pressed(sf::Keyboard::Right) && !block_movement){
            current_block->move_horizontal(true);
        }
    }
    // Rotation
    if(inputmgr->is_pressed_down(sf::Keyboard::Z) && !block_movement){
        current_block->rotate_left();
        current_block->apply_SRS(false);
    }
    if(inputmgr->is_pressed_down(sf::Keyboard::X) && !block_movement){
        current_block->rotate_right();
        current_block->apply_SRS(true);
    }
    // Hold
    if(inputmgr->is_pressed_down(sf::Keyboard::Space) && !block_movement){
        hold();
    }
    // Instant drop
    if(inputmgr->is_pressed_down(sf::Keyboard::Up)){
        current_block->instant_down();
        block_movement = true;
        hard_drop_sound.play();
    }
    // Accelerated drop
    if(inputmgr->is_pressed(sf::Keyboard::Down)){
        clock.change_interval(tick_interval * tick_modifier);
    }
    else{
        clock.change_interval(tick_interval);
    }
    // Usual drop
    if(clock.tick()){
        int rows_destroyed = remove_full_row();
        calculate_score(rows_destroyed);
        if(rows_destroyed == 0){
            bool success = current_block->move_down();
            if(!success){
                print_current_block();
            }
        }
    }

    if(current_block->do_collide()){
        game_state = 1;
        std::cout << "Game over!" << std::endl;
    }
}

void ttr::TetrisManager::print_current_block(){
    if(block_movement){
        score += current_block->count_rows_covered();
    }
    sf::Vector2i block_pos = current_block->get_position();
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++){
        if(!current_block->block_at(j, i)) continue;
        set_block_at(current_block->get_type(), block_pos.x + j, block_pos.y + i);
    }
    spawn_block();

    if(!block_movement){
        soft_drop_sound.play();
    }

    hold_available = true;
    block_movement = false;
}

void ttr::TetrisManager::spawn_block(ttr::BlockType specific){
    ttr::Block* block;
    if(specific == ttr::BlockType::Empty){
        block = block_pouch.get_next();
    }else{
        block = get_ptr(specific);
    }

    if(current_block != nullptr){
        delete current_block;
    }
    block->set_grid_ptr(&grid);
    current_block = block;
    current_block->set_position(4, 0);
}

int ttr::TetrisManager::remove_full_row(){
    int removed = 0;
    int full_row = get_full_row();
    while(full_row != -1){
        removed++;
        grid.erase(grid.begin() + full_row);
        grid.insert(grid.begin(), std::vector<bool>(grid_width, false));

        display_grid.erase(display_grid.begin() + full_row);
        display_grid.insert(display_grid.begin(), std::vector<ttr::BlockType>(grid_width, ttr::BlockType::Empty));
        
        full_row = get_full_row();
    }

    if(removed > 0){
        line_clear_sound.play();
    }

    return removed;
}

int ttr::TetrisManager::get_full_row(){
    for(int i = grid_height - 1; i >= 0; i--){
        if(std::all_of(grid[i].begin(), grid[i].end(), [](bool b){return b;})){
            return i;
        }
    }

    return -1;
}

void ttr::TetrisManager::hold(){
    if(!hold_available) return;

    if(holded == ttr::BlockType::Empty){
        holded = current_block->get_type();
        spawn_block();
    }
    else{
        ttr::BlockType replace = holded;
        holded = current_block->get_type();

        spawn_block(replace);
    }

    // Lock hold until a block is dropped
    hold_available = false;
}

void ttr::TetrisManager::calculate_score(int row_destroyed){
    if(row_destroyed == 0) return;

    if(row_destroyed == 1) score += 40;

    if(row_destroyed == 2) score += 100;

    if(row_destroyed == 3) score += 300;

    if(row_destroyed == 4) score += 1200;
}