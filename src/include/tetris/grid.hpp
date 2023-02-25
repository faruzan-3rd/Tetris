#ifndef __GRID_HPP_
#define __GRID_HPP_

#include <vector>
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"


namespace ttr{

    class Grid{
        private:
        int width;
        int height;
        int line_thickness;
        int square_size;

        sf::Color grid_color;
        sf::Vector2f position;
        sf::RenderWindow* window;
        std::vector<sf::RectangleShape> grid_vertical_lines, grid_horizontal_lines;

        public:
        Grid();
        Grid(int width, int height, int line_thickness, int square_size, sf::Color grid_color, sf::RenderWindow* window);

        void set_position(const sf::Vector2f& new_position);

        void build();

        void draw();

        int get_width() const;
        int get_height() const;
        int get_square_size() const;
        sf::Vector2f get_position() const;
    };

    ttr::Grid load_grid(const YAML::Node& config, sf::RenderWindow* window);
}


#endif