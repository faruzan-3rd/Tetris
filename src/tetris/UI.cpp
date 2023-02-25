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


    display_max = UIcfg["next_display_max"].as<int>();
    display_block_scale = UIcfg["display_block_scale"].as<float>();
    standard_block_size = config["Grid"]["square_size"].as<int>();
    block_interval = UIcfg["block_interval"].as<int>();
    block_offset = sf::Vector2f(UIcfg["block_offset"][0].as<int>(), UIcfg["block_offset"][1].as<int>());
    standard_block_scale = config["Sheet"]["scale"].as<int>();
    text_size = UIcfg["text_size"].as<int>();

    if(!font_fuckyou.loadFromFile(UIcfg["font"].as<std::string>())){
        std::cerr << "Error while reading font file." << std::endl;
    }

    hold_color = sf::Color(UIcfg["hold_txt_color"][0].as<int>(), UIcfg["hold_txt_color"][1].as<int>(), UIcfg["hold_txt_color"][2].as<int>());
    hold_txt.setCharacterSize(text_size);
    hold_txt.setString("Hold");
    hold_txt.setFillColor(hold_color);
    hold_txt.setPosition(sf::Vector2f(UIcfg["hold_txt_position"][0].as<int>(), UIcfg["hold_txt_position"][1].as<int>()));
    hold_txt.setFont(font_fuckyou);
    
    hold_box = sf::RectangleShape(sf::Vector2f(UIcfg["hold_box"][2].as<int>(), UIcfg["hold_box"][3].as<int>()));
    hold_box.setPosition(sf::Vector2f(UIcfg["hold_box"][0].as<int>(), UIcfg["hold_box"][1].as<int>()));
    hold_box.setOutlineColor(sf::Color(UIcfg["hold_box_color"][0].as<int>(), UIcfg["hold_box_color"][1].as<int>(), UIcfg["hold_box_color"][2].as<int>()));
    hold_box.setOutlineThickness(UIcfg["hold_box_thickness"].as<int>());
    hold_box.setFillColor(sf::Color::Transparent);

    next_color = sf::Color(UIcfg["next_txt_color"][0].as<int>(), UIcfg["next_txt_color"][1].as<int>(), UIcfg["next_txt_color"][2].as<int>());
    next_txt.setCharacterSize(text_size);
    next_txt.setString("Next");
    next_txt.setFillColor(next_color);
    next_txt.setPosition(sf::Vector2f(UIcfg["next_txt_position"][0].as<int>(), UIcfg["next_txt_position"][1].as<int>()));
    
    next_box = sf::RectangleShape(sf::Vector2f(UIcfg["next_box"][2].as<int>(), UIcfg["next_box"][3].as<int>()));
    next_box.setPosition(sf::Vector2f(UIcfg["next_box"][0].as<int>(), UIcfg["next_box"][1].as<int>()));
    next_box.setOutlineColor(sf::Color(UIcfg["next_box_color"][0].as<int>(), UIcfg["next_box_color"][1].as<int>(), UIcfg["next_box_color"][2].as<int>()));
    next_box.setOutlineThickness(UIcfg["next_box_thickness"].as<int>());
    next_box.setFillColor(sf::Color::Transparent);

    score_label_color = sf::Color(UIcfg["score_label_color"][0].as<int>(), UIcfg["score_label_color"][1].as<int>(), UIcfg["score_label_color"][2].as<int>());
    score_label.setCharacterSize(text_size);
    score_label.setString("Score");
    score_label.setFillColor(score_label_color);
    score_label.setPosition(sf::Vector2f(UIcfg["score_label_position"][0].as<int>(), UIcfg["score_label_position"][1].as<int>()));

    score_color = sf::Color(UIcfg["score_txt_color"][0].as<int>(), UIcfg["score_txt_color"][1].as<int>(), UIcfg["score_txt_color"][2].as<int>());
    score_txt.setCharacterSize(UIcfg["score_txt_size"].as<int>());
    score_txt.setString("00000000");
    score_txt.setFillColor(score_color);
    score_txt.setPosition(sf::Vector2f(UIcfg["score_txt_position"][0].as<int>(), UIcfg["score_txt_position"][1].as<int>()));

    start.setString("Press enter to start");
    start.setPosition(sf::Vector2f(220, 50));
    start.setCharacterSize(20);
    start.setFillColor(sf::Color::White);

    gameover.setString("Game over");
    gameover.setPosition(sf::Vector2f(250, 50));
    gameover.setCharacterSize(35);
    gameover.setFillColor(sf::Color::White);

    score_digits = UIcfg["score_digits"].as<int>();

    sprites = ttr::load_sprites(UIcfg["Blocks"], *texture, false);
}


void ttr::NextAndHold::draw(){
    if(*game_state == -1){
        start.setFont(font_fuckyou);
        window->draw(start);
    }
    else{

        hold_txt.setFont(font_fuckyou);
        next_txt.setFont(font_fuckyou);
        score_label.setFont(font_fuckyou);
        score_txt.setFont(font_fuckyou);

        std::stringstream wos;
        wos << std::setfill('0') << std::setw(score_digits) << *score;
        score_txt.setString(wos.str());

        for(int i = 0; i < std::min((int)(next_list->size()), display_max); i++){
            sprites[next_list->at(i)].setPosition(next_box.getPosition() + block_offset + sf::Vector2f(0, block_interval * i));
            window->draw(sprites[next_list->at(i)]);
        }

        sprites[*hold].setPosition(hold_box.getPosition() + block_offset);
        window->draw(sprites[*hold]);

        window->draw(next_txt);
        window->draw(hold_txt);
        window->draw(next_box);
        window->draw(hold_box);
        window->draw(score_label);
        window->draw(score_txt);
        
        if(*game_state == 1){
            gameover.setFont(font_fuckyou);
            window->draw(gameover);
        }
    }
}