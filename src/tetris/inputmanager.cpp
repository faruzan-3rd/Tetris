#include "tetris/tetris.hpp"


ttr::InputManager::InputManager(){
}

bool ttr::InputManager::is_pressed_down(const sf::Keyboard::Key& key){
    return pressed_down[key];
}

bool ttr::InputManager::is_pressed(const sf::Keyboard::Key& key){
    return sf::Keyboard::isKeyPressed(key);
}

void ttr::InputManager::set_pressed_down(const sf::Keyboard::Key& key){
    pressed_down[key] = true;
}

void ttr::InputManager::reset(){
    pressed_down.clear();
}
