#include "tetris/UI.hpp"


ttr::NextAndHold::NextAndHold(){};

ttr::NextAndHold::NextAndHold(sf::RenderWindow* window_, const std::vector<ttr::BlockType>* next_list_, const ttr::BlockType* hold_, const long* score_, int* game_state_, YAML::Node config_, sf::Texture* texture_){
    this->window = window_;
    this->next_list = next_list_;
    this->hold = hold_;
    this->config = config_;
    this->texture = texture_;
    this->score = score_;
    this->game_state = game_state_;
    auto UIcfg = config["UI"];

    scheme = was::UIScheme(UIcfg["scheme"].as<std::string>(), window);
    start = was::UIScheme(UIcfg["start"].as<std::string>(), window);
    gameover = was::UIScheme(UIcfg["gameover"].as<std::string>(), window);

    display_max = UIcfg["next_display_max"].as<int>();
    display_block_scale = UIcfg["display_block_scale"].as<float>();
    standard_block_size = config["Grid"]["square_size"].as<int>();
    block_interval = UIcfg["block_interval"].as<int>();
    block_offset = sf::Vector2f(UIcfg["block_offset"][0].as<int>(), UIcfg["block_offset"][1].as<int>());
    standard_block_scale = config["Sheet"]["scale"].as<int>();

    score_digits = UIcfg["score_digits"].as<int>();

    sprites = ttr::load_sprites(UIcfg["Blocks"], *texture, false);
}


void ttr::NextAndHold::draw(){
    if(*game_state == -1){
        start.draw();
    }
    else{
        std::stringstream wos;
        wos << std::setfill('0') << std::setw(score_digits) << *score;
        scheme.get_ptr_by_name("score")->set_text(wos.str());
        
        sf::Vector2f next_box_pos = scheme.get_ptr_by_name("next_box")->get_position();
        sf::Vector2f hold_box_pos = scheme.get_ptr_by_name("hold_box")->get_position();

        for(int i = 0; i < std::min((int)(next_list->size()), display_max); i++){
            sprites[next_list->at(i)].setPosition(next_box_pos + block_offset + sf::Vector2f(0, block_interval * i));
            window->draw(sprites[next_list->at(i)]);
        }

        sprites[*hold].setPosition(hold_box_pos + block_offset);
        window->draw(sprites[*hold]);

        scheme.draw();
        
        if(*game_state == 1){
            gameover.draw();
        }
    }
}
