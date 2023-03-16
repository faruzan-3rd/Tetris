#ifndef __UI_HPP
#define __UI_HPP

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "tetris/enum.hpp"
#include "tetris/blocks.hpp"
#include "tetris/assets.hpp"
#include "wastils/UI.hpp"


namespace ttr{

    class NextAndHold{
        private:
        YAML::Node config;
        sf::RenderWindow* window;

        was::UIScheme scheme;
        was::UIScheme start, gameover;

        const long* score;
        const ttr::BlockType* hold;
        const std::vector<ttr::BlockType>* next_list;

        int display_max;
        int standard_block_size;
        int standard_block_scale;
        int block_interval;
        float display_block_scale;
        sf::Vector2f block_offset;
        int score_digits;
        int* game_state;

        sf::Texture* texture;
        std::map<ttr::BlockType, sf::Sprite> sprites;

        public:
        NextAndHold();
        NextAndHold(sf::RenderWindow* window_, const std::vector<ttr::BlockType>* next_list_, const ttr::BlockType* hold, const long* score_, int* game_state_, YAML::Node config_, sf::Texture* texture);

        void draw();

    };
}


#endif