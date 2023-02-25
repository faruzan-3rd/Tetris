#ifndef __BLOCKS_HPP
#define __BLOCKS_HPP

#include <iostream>
#include "tetris/enum.hpp"
#include "SFML/Graphics.hpp"

namespace ttr{
    constexpr int UP = 0;
    constexpr int RIGHT = 1;
    constexpr int DOWN = 2;
    constexpr int LEFT = 3;

    class Block{
        protected:
        ttr::BlockType btype = ttr::BlockType::Empty;
        bool shape[4][4] = {{false, false, false, false},
                            {false, false, false, false},
                            {false, false, false, false},
                            {false, false, false, false}};
        int posx = 0;
        int posy = 0;
        int offset_x = 0;
        int offset_y = 0;
        int rotation_state = 0;
        std::vector<std::vector<bool>>* grid_ptr;

        void update_shape(bool new_shape[4][4]);

        void transpose(int size=3);

        void reverse_row(int size=3);

        public:
        Block();

        void set_position(int x, int y);

        sf::Vector2i get_position();

        void set_type(ttr::BlockType type_);

        void draw(sf::RenderWindow* window, const int& block_size, std::map<ttr::BlockType, sf::Sprite>& sprites, std::map<ttr::BlockType, sf::Sprite>& shadow_sprites, const sf::Vector2f& offset, const float& scale=1.0f, bool shadow=false);

        bool move_down();

        void instant_down();

        bool move_horizontal(bool right);

        ttr::BlockType get_type();

        bool do_collide();

        bool block_at(const int& x, const int& y) const;

        void set_grid_ptr(std::vector<std::vector<bool>>* grid);

        int find_max_y();

        int count_rows_covered();


        virtual void rotate_right();

        virtual void rotate_left();

        // Super Rotation System
        // Moves the block if it hits something when rotated
        virtual bool SRS1(bool right);

        virtual bool SRS2(bool right);

        virtual bool SRS3(bool right);

        virtual bool SRS4(bool right);

        void apply_SRS(bool right_rotation);
    };

    namespace TBlock{

        class I: public Block{
            public:
            I();

            void rotate_left() override;
            void rotate_right() override;

            bool SRS1(bool right) override;
            bool SRS2(bool right) override;
            bool SRS3(bool right) override;
            bool SRS4(bool right) override;
        };

        class J: public Block{
            public:
            J();
        };

        class L: public Block{
            public:
            L();
        };

        class O: public Block{
            public:
            O();

            void rotate_left() override;
            void rotate_right() override;
        };

        class S: public Block{
            public:
            S();
        };

        class T: public Block{
            public:
            T();
        };   

        class Z: public Block{
            public:
            Z();
        };
    }

    ttr::Block* get_ptr(ttr::BlockType btype);
}

#endif