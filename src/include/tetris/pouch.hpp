#ifndef __POUCH_HPP
#define __POUCH_HPP

#include <queue>
#include <random>
#include <iostream>
#include <algorithm>
#include "tetris/enum.hpp"
#include "tetris/blocks.hpp"


namespace ttr{
    constexpr int pouch_max = 4;

    class Pouch{
        private:
        std::queue<std::queue<ttr::BlockType>> pouch_que;
        std::vector<ttr::BlockType> next_list;

        public:
        Pouch();

        ttr::Block* get_next();

        const std::vector<ttr::BlockType>* get_next_list();

        private:
        void generate_next_pouch();
    };
}


#endif