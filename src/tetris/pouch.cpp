#include "tetris/pouch.hpp"


ttr::Pouch::Pouch(){
    for(int i = 0; i < pouch_max; i++){
        generate_next_pouch();
    }
}

ttr::Block* ttr::Pouch::get_next(){
    ttr::BlockType next = pouch_que.front().front();
    pouch_que.front().pop();
    if(pouch_que.front().empty()){
        pouch_que.pop();
        generate_next_pouch();
    }
    if(!next_list.empty()) next_list.erase(next_list.begin());

    return ttr::get_ptr(next);
}

const std::vector<ttr::BlockType>* ttr::Pouch::get_next_list(){
    return &next_list;
}

void ttr::Pouch::generate_next_pouch(){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    ttr::BlockType blocks[sizeof(ttr::All_blocks)/sizeof(ttr::All_blocks[0])];
    std::copy(std::begin(ttr::All_blocks), std::end(ttr::All_blocks), std::begin(blocks));
    std::shuffle(std::begin(blocks), std::end(blocks), engine);

    std::queue<ttr::BlockType> next_pouch;
    for(size_t i = 0; i < (sizeof(blocks)/sizeof(blocks[0])); i++){
        next_pouch.push(blocks[i]);
        next_list.push_back(blocks[i]);
    }
    pouch_que.push(next_pouch);
}
