#include "tetris/assets.hpp"


std::map<ttr::BlockType, sf::Sprite> ttr::load_sprites(const YAML::Node& config, sf::Texture& texture, bool fixed_size){
    std::map<ttr::BlockType, sf::Sprite> ret;

    int size = (fixed_size ? config["size"].as<int>() : 0);
    int scale = config["scale"].as<int>();
    int alpha = config["alpha"].as<int>();

    auto sprites = config["Sprites"];

    for(auto block_type : ttr::All_types){
        auto pos = sprites[ttr::string_type.at(block_type)];
        int width = (fixed_size ? size : pos[2].as<int>());
        int height = (fixed_size ? size : pos[3].as<int>());
        sf::Sprite sprite(texture, sf::IntRect(pos[0].as<int>(), pos[1].as<int>(), width, height));
        sprite.setScale(sf::Vector2f(scale, scale));
        sprite.setColor(sf::Color(255, 255, 255, alpha));
        ret[block_type] = sprite;
    }

    return ret;
}