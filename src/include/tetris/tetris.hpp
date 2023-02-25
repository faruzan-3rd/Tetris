#ifndef __TETRIS_HPP_
#define __TETRIS_HPP_

#include <set>
#include <vector>
#include <string>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "tetris/UI.hpp"
#include "tetris/grid.hpp"
#include "tetris/pouch.hpp"
#include "tetris/assets.hpp"
#include "tetris/blocks.hpp"


namespace ttr{
    class GameClock{
        private:
        float tick_interval;
        float _delta;
        int _last_tick;
        float _timer;
        sf::Clock clock;

        public:
        GameClock();
        GameClock(float tick_interval);

        void update();

        bool tick();

        void change_interval(float new_interval);
    };

    class InputManager{
        private:
        std::map<sf::Keyboard::Key, bool> pressed_down;

        public:
        InputManager();

        bool is_pressed_down(const sf::Keyboard::Key& key);

        bool is_pressed(const sf::Keyboard::Key& key);

        void set_pressed_down(const sf::Keyboard::Key& key);

        void reset();
    };

    class TetrisManager{
        private:
        int grid_width;
        int grid_height;
        int square_size;
        int square_scale;
        float tick_interval;
        float tick_modifier;
        bool hold_available;
        // Avoid the block from getting moved after instant drop
        bool block_movement;
        bool shadow_enabled;
        long score;
        int game_state;

        std::vector<std::vector<bool>> grid;
        std::vector<std::vector<BlockType>> display_grid;

        ttr::Grid grid_lines;
        YAML::Node* config;

        std::map<ttr::BlockType, sf::Sprite> sprites, shadow_sprites;

        sf::RenderWindow* window;
        sf::Texture sprite_sheet;

        ttr::GameClock clock;
        ttr::InputManager* inputmgr;
        ttr::GameClock inputClock;

        ttr::Block* current_block;
        ttr::BlockType holded;
        ttr::Pouch block_pouch;

        ttr::NextAndHold gameui;

        sf::SoundBuffer hard_drop, line_clear, soft_drop;
        sf::Music bgm;
        sf::Sound hard_drop_sound, line_clear_sound, soft_drop_sound;

        public:
        TetrisManager();
        TetrisManager(YAML::Node* config, sf::RenderWindow* window, ttr::InputManager* inputmanager);
        ~TetrisManager();

        bool set_block_at(ttr::BlockType btype, int x, int y);

        bool remove_block_at(int x, int y);

        void draw();

        void update();

        void spawn_block(ttr::BlockType specific=ttr::BlockType::Empty);

        void print_current_block();

        int remove_full_row();

        int get_full_row();

        void hold();

        void calculate_score(int row_destroyed);
    };
}


#endif