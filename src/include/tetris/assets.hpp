#ifndef __ASSETS_HPP_
#define __ASSETS_HPP_

#include <map>
#include <iostream>
#include "tetris/enum.hpp"
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"



namespace ttr{
    
    std::map<ttr::BlockType, sf::Sprite> load_sprites(const YAML::Node& config, sf::Texture& texture, bool fixed_size=true);
    
}


#endif