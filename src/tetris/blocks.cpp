#include "tetris/blocks.hpp"


ttr::Block::Block(){
}

void ttr::Block::set_position(int x, int y){
    posx = x;
    posy = y;
}

void ttr::Block::set_type(ttr::BlockType type_){
    btype = type_;
}

void ttr::Block::update_shape(bool new_shape[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            shape[i][j] = new_shape[i][j];
        }
    }
}

void ttr::Block::draw(sf::RenderWindow* window, const int& block_size, std::map<ttr::BlockType, sf::Sprite>& sprites, std::map<ttr::BlockType, sf::Sprite>& shadow_sprites, const sf::Vector2f& offset, const float& scale, bool shadow){
    int shadow_offset_y = (shadow ? find_max_y() : 0);
    sf::Sprite target = (shadow ? shadow_sprites[btype] : sprites[btype]);
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++){
        if(shape[i][j]){
            target.setScale(sf::Vector2f(scale, scale));
            target.setPosition(offset + sf::Vector2f((j + posx + offset_x) * block_size, (i + posy + offset_y + shadow_offset_y) * block_size));
            window->draw(target);
        }
    }
}

bool ttr::Block::move_down(){
    posy++;
    if(do_collide()){
        posy--;
        return false;
    }

    return true;
}

void ttr::Block::instant_down(){
    while(move_down());
}

bool ttr::Block::move_horizontal(bool right){
    int dir = (right ? 1 : -1);

    posx += dir;
    if(do_collide()){
        posx -= dir;
        return false;
    }

    return true;
}

bool ttr::Block::do_collide(){
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++){
        int newx = posx + j + offset_x, newy = posy + i + offset_y;

        bool hit_or_out = !(0 <= newx && newx < (*grid_ptr)[0].size() && 0 <= newy && newy < grid_ptr->size()) || (*grid_ptr)[newy][newx];
        if(shape[i][j] && hit_or_out){
            return true;
        }
    }

    return false;
}

sf::Vector2i ttr::Block::get_position(){
    return sf::Vector2i(posx, posy);
}

ttr::BlockType ttr::Block::get_type(){
    return btype;
}

bool ttr::Block::block_at(const int& x, const int& y) const{
    return shape[y][x];
}

void ttr::Block::rotate_left(){
    reverse_row();
    transpose();
    rotation_state = (rotation_state - 1 + 4) % 4;
}

void ttr::Block::rotate_right(){
    transpose();
    reverse_row();
    rotation_state = (rotation_state + 1 + 4) % 4;
}

void ttr::Block::transpose(int size){
    bool original[4][4];
    for(int i = 0; i < 4; i++){
        std::copy(std::begin(shape[i]), std::end(shape[i]), std::begin(original[i]));
    }

    for(int i = 0; i < size; i++) for(int j = 0; j < size; j++){
        shape[j][i] = original[i][j];
    }
}

void ttr::Block::reverse_row(int size){
    for(int i = 0; i < size; i++){
        std::reverse(std::begin(shape[i]), std::begin(shape[i]) + size);
    }
}

void ttr::Block::set_grid_ptr(std::vector<std::vector<bool>>* grid){
    grid_ptr = grid;
}

int ttr::Block::find_max_y(){
    while(!do_collide()){
        offset_y++;
    }

    int ret = offset_y - 1;
    offset_y = 0;
    return ret;
}

int ttr::Block::count_rows_covered(){
    int count = 0;
    for(int i = 0; i < 4; i++){
        count += std::any_of(std::begin(shape[i]), std::end(shape[i]), [](bool b){return b;});
    }
    return count;
}

bool ttr::Block::SRS1(bool right){
    if(rotation_state == ttr::RIGHT){  // 90deg
        offset_x -= 1;
    }
    else if(rotation_state == ttr::LEFT){  // -90deg
        offset_x += 1;
    }
    else if(rotation_state == ttr::UP){
        if(right){  // before: LEFT
            offset_x -= 1;
        }
        else{  // before: RIGHT
            offset_x += 1;
        }
    }
    else if(rotation_state == ttr::DOWN){
        if(right){  // before: RIGHT
            offset_x += 1;
        }
        else{  // before: LEFT
            offset_x -= 1;
        }
    }

    return do_collide();
}

bool ttr::Block::SRS2(bool right){
    if(rotation_state == ttr::RIGHT || rotation_state == ttr::LEFT){
        offset_y -= 1;
    }
    else{
        offset_y += 1;
    }

    return do_collide();
}

bool ttr::Block::SRS3(bool right){
    offset_x = 0;
    offset_y = 0;

    if(rotation_state == ttr::RIGHT || rotation_state == ttr::LEFT){
        offset_y += 2;
    }
    else{
        offset_y -= 2;
    }

    return do_collide();
}

bool ttr::Block::SRS4(bool right){
    return SRS1(right);
}

void ttr::Block::apply_SRS(bool right_rotation){
    offset_x = 0;
    offset_y = 0;
    bool ret = false;
    if(!do_collide()) ret = true;

    if(!ret && !SRS1(right_rotation)) ret = true;

    if(!ret && !SRS2(right_rotation)) ret = true;

    if(!ret && !SRS3(right_rotation)) ret = true;

    if(!ret && !SRS4(right_rotation)) ret = true;
    
    if(ret){
        posx += offset_x;
        posy += offset_y;
    }
    else{  // if the block is really stuck
        if(right_rotation){
            rotate_left();
        }
        else{
            rotate_right();
        }
    }
    offset_x = 0;
    offset_y = 0;
}



ttr::TBlock::J::J(){
    bool blockshape[4][4] = {{true,  false, false, false},
                            { true,  true,  true, false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::J);
}

ttr::TBlock::L::L(){
    bool blockshape[4][4] = {{false, false, true, false},
                            { true,  true,  true, false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::L);
}

ttr::TBlock::S::S(){
    bool blockshape[4][4] = {{false, true, true,  false},
                            { true,  true, false, false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::S);
}

ttr::TBlock::T::T(){
    bool blockshape[4][4] = {{false, true,  false, false},
                            { true,  true,  true,  false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::T);
}

ttr::TBlock::Z::Z(){
    bool blockshape[4][4] = {{true,  true,  false, false},
                            { false, true,  true,  false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::Z);
}


ttr::TBlock::O::O(){
    bool blockshape[4][4] = {{true,  true, false, false},
                            { true,  true, false, false},
                            { false, false, false, false},
                            { false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::O);
}

void ttr::TBlock::O::rotate_left(){
    rotation_state = (rotation_state - 1) % 4;
}

void ttr::TBlock::O::rotate_right(){
    rotation_state = (rotation_state + 1) % 4;
}


ttr::TBlock::I::I(){
    bool blockshape[4][4] = {{false, false, false, false},
                            { true,  true,  true,  true},
                            {false, false, false, false},
                            {false, false, false, false}};
    update_shape(blockshape);
    set_type(ttr::BlockType::I);
}

void ttr::TBlock::I::rotate_left(){
    reverse_row(4);
    transpose(4);
    rotation_state = (rotation_state - 1 + 4) % 4;
}

void ttr::TBlock::I::rotate_right(){
    transpose(4);
    reverse_row(4);
    rotation_state = (rotation_state + 1) % 4;
}

bool ttr::TBlock::I::SRS1(bool right){
    offset_x = 0;
    offset_y = 0;
    if(rotation_state == ttr::RIGHT){
        offset_x = 1;
    }
    else if(rotation_state == ttr::LEFT){
        offset_x = -1;
    }
    else if(rotation_state == ttr::UP){
        if(right){  // before: LEFT
            offset_x = -2;
        }
        else{  // before: RIGHT
            offset_x = 2;
        }
    }
    else if(rotation_state == ttr::DOWN){
        if(right){  // before: RIGHT
            offset_x = 1;
        }
        else{  // before: LEFT
            offset_x = -1;
        }
    }
    return do_collide();
}

bool ttr::TBlock::I::SRS2(bool right){
    if(rotation_state == ttr::RIGHT){
        offset_x = -2;
    }
    else if(rotation_state == ttr::LEFT){
        offset_x = 2;
    }
    else if(rotation_state == ttr::UP){
        if(right){  // before: LEFT
            offset_x = 1;
        }
        else{  // before: RIGHT
            offset_x = -1;
        }
    }
    else if(rotation_state == ttr::DOWN){
        if(right){  // before: RIGHT
            offset_x = -2;
        }
        else{  // before: LEFT
            offset_x = 2;
        }
    }
    return do_collide();
}

bool ttr::TBlock::I::SRS3(bool right){
    int mov_distance = (right ? 1 : 2);
    if(rotation_state == ttr::RIGHT || rotation_state == ttr::LEFT){
        I::SRS1(right);
        offset_y += (rotation_state == ttr::RIGHT ? 1 : -1) * mov_distance;
    }
    else if(rotation_state == ttr::UP){
        if(right){  // before: LEFT
            I::SRS2(right);
            offset_y += mov_distance;
        }
        else{  // before: RIGHT
            I::SRS1(right);
            offset_y -= mov_distance;
        }
    }
    else if(rotation_state == ttr::DOWN){
        if(right){  // before: RIGHT
            I::SRS1(right);
            offset_y -= mov_distance;
        }
        else{  // before: LEFT
            I::SRS2(right);
            offset_y += mov_distance;
        }
    }

    return do_collide();
}

bool ttr::TBlock::I::SRS4(bool right){
    int mov_distance = (right ? 2 : 1);
    if(rotation_state == ttr::RIGHT || rotation_state == ttr::LEFT){
        I::SRS2(right);
        offset_y += (rotation_state == ttr::RIGHT ? -1 : 1) * mov_distance;
    }
    else if(rotation_state == ttr::UP){
        if(right){  // before: LEFT
            I::SRS1(right);
            offset_y -= mov_distance;
        }
        else{  // before: RIGHT
            I::SRS2(right);
            offset_y += mov_distance;
        }
    }
    else if(rotation_state == ttr::DOWN){
        if(right){  // before: RIGHT
            I::SRS2(right);
            offset_y += mov_distance;
        }
        else{  // before: LEFT
            I::SRS1(right);
            offset_y -= mov_distance;
        }
    }

    return do_collide();
}

ttr::Block* ttr::get_ptr(ttr::BlockType btype){
    ttr::Block* block = nullptr;
    switch(btype){
        case ttr::BlockType::I:
            block = new ttr::TBlock::I();
            break;
        
        case ttr::BlockType::J:
            block = new ttr::TBlock::J();
            break;

        case ttr::BlockType::L:
            block = new ttr::TBlock::L();
            break;

        case ttr::BlockType::O:
            block = new ttr::TBlock::O();
            break;

        case ttr::BlockType::S:
            block = new ttr::TBlock::S();
            break;

        case ttr::BlockType::T:
            block = new ttr::TBlock::T();
            break;

        case ttr::BlockType::Z:
            block = new ttr::TBlock::Z();
            break;

        default:
            std::cerr << "Error while generating block" << std::endl;
            break;
    };

    return block;
}