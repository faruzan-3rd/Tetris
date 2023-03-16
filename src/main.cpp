#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "tetris/tetris.hpp"
#include "wastils/config.hpp"


int main(){
    std::cout << "Welcome to tetris! " << std::endl;
    YAML::Node config;
    if(was::load_config(config, "assets/config.yaml") == 1){
        return 1;
    }

    // Basic settings
    int sc_w = config["Window"]["width"].as<int>(), sc_h = config["Window"]["height"].as<int>();
    std::string title = config["Title"].as<std::string>();
    sf::RenderWindow window(sf::VideoMode(sc_w, sc_h), title, sf::Style::Titlebar | sf::Style::Close);

    std::cout << "Window generated" << std::endl;

    window.setFramerateLimit(config["Game"]["framerate"].as<int>());
    window.setKeyRepeatEnabled(false);
    sf::Color backgroundColor(config["Background"]["r"].as<int>(), config["Background"]["g"].as<int>(), config["Background"]["b"].as<int>());
    sf::Image icon;
    icon.loadFromFile("assets/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    ttr::InputManager input;
    ttr::TetrisManager manager(&config, &window, &input);

    std::cout << "Loading finished" << std::endl;

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