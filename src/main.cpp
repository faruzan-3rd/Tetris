#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "tetris/tetris.hpp"
#include "tetris/config.hpp"


int main(){
    YAML::Node config;
    if(ttr::load_config(config, "assets/config.yaml") == 1){
        return 1;
    }

    // Basic settings
    int sc_w = config["Window"]["width"].as<int>(), sc_h = config["Window"]["height"].as<int>();
    std::string title = config["Title"].as<std::string>();
    sf::RenderWindow window(sf::VideoMode(sc_w, sc_h), title, sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(config["Game"]["framerate"].as<int>());
    window.setKeyRepeatEnabled(false);
    sf::Color backgroundColor(config["Background"]["r"].as<int>(), config["Background"]["g"].as<int>(), config["Background"]["b"].as<int>());

    ttr::InputManager input;
    ttr::TetrisManager manager(&config, &window, &input);

    while (window.isOpen())
    {
        input.reset();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            if(event.type == sf::Event::KeyPressed){
                input.set_pressed_down(event.key.code);
            }
        }
        window.clear(backgroundColor);
        manager.update();
        manager.draw();
        window.display();
    }    
}