#include "tetris/tetris.hpp"


ttr::GameClock::GameClock(){}

ttr::GameClock::GameClock(float tick_interval){
    this->tick_interval = tick_interval;
    _last_tick = 0;
    _delta = 0;
    clock = sf::Clock();
    _timer = 0;
}

void ttr::GameClock::update(){
    _delta = (clock.getElapsedTime().asMilliseconds() - _last_tick) / 1000.0f;
    _last_tick = clock.getElapsedTime().asMilliseconds();
    _timer += _delta;
}

bool ttr::GameClock::tick(){
    if(_timer >= tick_interval){
        _timer = 0;
        return true;
    }

    return false;
}

void ttr::GameClock::change_interval(float new_interval){
    tick_interval = new_interval;
}